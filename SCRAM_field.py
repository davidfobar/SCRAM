## define classes to simulate a radiation field and a detector

#define the simulated radition field
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from filterpy.kalman import KalmanFilter
from scipy.optimize import minimize

#define a class to hold a single scattered detector and its related methods
class Detector:
  def __init__(self, id, positionMeasurementNoise=0.2, positionProcessVariance=1e-6, expectedDecayRate=0.1, decayProcessVariance=0.01, nGateways=4):
    self.id = id
    self.predictedDoseRateMeasurements = []
    self.predictedDoseRateUncertainties = []
    self.predictionTimes = []
    self.predictedX = []
    self.predictedY = []
    self.predictedR = 1
    self.posUncertainty = []

    #initialize the kalman filter for estimating the time of flight from each gateway
    self.timeOfFlightFilter = KalmanFilter(dim_x=nGateways, dim_z=nGateways)
    self.timeOfFlightFilter.x = np.zeros(nGateways) #initial state (time of flight to each gateway)
    self.timeOfFlightFilter.F = np.eye(nGateways) #state transition matrix
    self.timeOfFlightFilter.H = np.eye(nGateways) #measurement function
    self.timeOfFlightFilter.R = np.eye(nGateways) * 2 # Measurement noise - this should be near what is expiemntally measured, estimated to be 50 nanoseconds
    self.timeOfFlightFilter.Q = np.eye(nGateways) * 1e-6  # Process noise - static placement, so this is low

    #initialize the kalman filter for estimating the detector position
    self.positionFilter = KalmanFilter(dim_x=4, dim_z=2)
    self.positionFilter.x = np.array([0, 0, 0, 0]) #initial state (x, y, xdot, ydot)
    self.positionFilter.F = np.array([[1, 0, 1, 0], #state transition matrix
                                      [0, 1, 0, 1],
                                      [0, 0, 1, 0],
                                      [0, 0, 0, 1]])
    self.positionFilter.H = np.array([[1, 0, 0, 0], #measurement function
                                      [0, 1, 0, 0]])
    self.positionFilter.R = np.eye(2) * positionMeasurementNoise # Measurement noise - this should be near what is expiemntally measured, estimated to be 200 meters
    self.positionFilter.Q = np.eye(4) * positionProcessVariance  # Process noise - static placement, so this is low

    #initialize the kalman filter for dose rate estimation
    self.doseRateFilter = KalmanFilter(dim_x=2, dim_z=1)
    self.doseRateFilter.x = np.array([0, 1]) #initial state (dose rate and decay rate)
    self.doseRateFilter.F = np.array([[1, 1], [0, 1]]) #state transition matrix
    self.doseRateFilter.H = np.array([[1, 0]]) #measurement function
    self.doseRateFilter.R = np.array([[1e-2]]) #initial measurement noise
    self.doseRateFilter.Q = np.array([[0, 0],[0, decayProcessVariance*expectedDecayRate**2]]) #process noise
  
  def plotDoseRate(self, trueDoseRates):
    #create a figure with subplots for each time
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.errorbar(self.predictionTimes, self.predictedDoseRateMeasurements, yerr=self.predictedDoseRateUncertainties, fmt='o', label='Predicted Dose Rate')
    ax.plot(self.predictionTimes, trueDoseRates, 'r', label='True Dose Rate')
    ax.set_xlabel('Time') 
    ax.set_ylabel('Dose Rate')
    ax.set_title('Dose Rate vs Time')
    ax.legend()
    plt.show()

  def plotDistanceError(self, trueX, trueY):
    #calculate the distance between the true position and the predicted position
    distance = np.sqrt((np.array(self.predictedX) - trueX)**2 + (np.array(self.predictedY) - trueY)**2)
    uncertainty = np.array(self.posUncertainty)
    plt.errorbar(self.predictionTimes, distance*1000, yerr=uncertainty*1000, fmt='o', color='k')
    plt.plot(self.predictionTimes, np.zeros(len(self.predictionTimes)), 'k--')
    plt.xlabel('Time')
    plt.ylabel('Distance from True Position (m)')
    plt.title('Distance from True Position')
    plt.show()

  def update(self, doseMeasurement, doseSigma, differentialToFs, time, gateways):
    #if this is the first measurement, set the initial state to the measurement
    if len(self.predictionTimes) == 0:
      self.positionFilter.x = np.array([0, 0, 0, 0])
      self.doseRateFilter.x = np.array([doseMeasurement, 1])

    self.predictionTimes.append(time)

    #update the dose rate filter
    self.doseRateFilter.R = np.array([[doseSigma]])
    self.doseRateFilter.predict()
    self.doseRateFilter.update(doseMeasurement)
    self.predictedDoseRateMeasurements.append(self.doseRateFilter.x[0])
    self.predictedDoseRateUncertainties.append(np.sqrt(self.doseRateFilter.P[0,0]))

    #update the time of flight filter
    self.timeOfFlightFilter.predict()
    self.timeOfFlightFilter.update(differentialToFs)
    differentialToFs = self.timeOfFlightFilter.x
     
    if len(self.predictedX) == 0:
      initial_guess = [0.0, 0.0, 1]
    else:
      initial_guess = [self.predictedX[-1], self.predictedY[-1], self.predictedR]
    #result = minimize(diffToFerror, initial_guess, args=(differentialToFs, gateways), method='nelder-mead', options={'maxiter': 100})
    result = minimize(diffToFerror, initial_guess, args=(differentialToFs, gateways), method='SLSQP', options={'ftol': 1e-2})

    predictedX, predictedY, self.predictedR = result.x

    #update the position filter
    #use uncertainty in the time of flight to estimate the uncertainty in the position
    self.positionFilter.R = np.eye(2) * np.sqrt(np.sum(self.timeOfFlightFilter.P)) * 0.1
    self.positionFilter.predict()
    self.positionFilter.update(np.array([predictedX, predictedY]))
    self.predictedX.append(self.positionFilter.x[0])
    self.predictedY.append(self.positionFilter.x[1])
    self.posUncertainty.append(np.sqrt(self.positionFilter.P[0,0] + self.positionFilter.P[1,1]))

class Gateway:
  def __init__(self, x, y):
    self.x = x
    self.y = y

## Helper functions
def diffToFerror(vars, dTof, gateways):
    x, y, r = vars
    totalError = 0
    for tof, gateway in zip(dTof, gateways):
        tof *= 1e-9  # Convert from ns to s
        error = (r + tof * 299792458/1000)**2 - (x - gateway.x)**2 - (y - gateway.y)**2
        totalError += error**2
    return totalError

def minimumRadius(r, c, dtofs, gateways):
    result = 0
    for tof, gateway in zip(dtofs, gateways):
        result += (r + tof * c)**2 - (gateways[0].x - gateway.x)**2 - (gateways[0].y - gateway.y)**2
    return result

def metersToDeltaLat(meters):
  return meters / 111111

def metersToDeltaLong(meters, lat):
  return np.abs(meters / (111111 * np.cos(lat)))

def deltaLatToMeters(deltaLat):
  return deltaLat * 111111

def deltaLongToMeters(deltaLong, lat):
  return np.abs(deltaLong * 111111 * np.cos(lat))