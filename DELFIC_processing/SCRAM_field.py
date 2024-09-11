## define classes to simulate a radiation field and a detector

#define the simulated radition field
from math import e
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from filterpy.kalman import KalmanFilter
from scipy.optimize import minimize

measurementBufferSize = 1000

#define a class to hold a single scattered detector and its related methods
class Detector:
  def __init__(self, id, gateways, positionMeasurementNoise=0.2, positionProcessVariance=1e-6, expectedDecayRate=0.1, decayProcessVariance=0.01):
    self.id = id
    self.estimatedDoseRateMeasurements = np.zeros(measurementBufferSize)
    self.estimatedDoseRateUncertainties = np.zeros(measurementBufferSize)
    self.predictionTimes = np.zeros(measurementBufferSize)
    self.measuredR = 1
    self.measuredX = np.zeros(measurementBufferSize)
    self.measuredY = np.zeros(measurementBufferSize)
    self.posUncertainty = np.zeros(measurementBufferSize)
    self.gateways = gateways
    self.updateIdx = 0

    nGateways = len(gateways)

    #initialize the kalman filter for estimating the time of flight from each gateway
    self.timeOfFlightFilter = KalmanFilter(dim_x=nGateways, dim_z=nGateways)
    self.timeOfFlightFilter.x = np.zeros(nGateways) #initial state (time of flight to each gateway)
    self.timeOfFlightFilter.F = np.eye(nGateways) #state transition matrix
    self.timeOfFlightFilter.H = np.eye(nGateways) #measurement function
    self.timeOfFlightFilter.R = np.eye(nGateways) * .5 # Measurement noise - this should be near what is expiemntally measured, estimated to be 50 nanoseconds
    self.timeOfFlightFilter.Q = np.eye(nGateways) * 1e-6  # Process noise - static placement, so this is low

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
    ax.errorbar(self.predictionTimes[:self.updateIdx], self.estimatedDoseRateMeasurements[:self.updateIdx], yerr=self.estimatedDoseRateUncertainties[:self.updateIdx], fmt='o', label='Predicted Dose Rate')
    ax.plot(self.predictionTimes[:self.updateIdx], trueDoseRates[:self.updateIdx], 'r', label='True Dose Rate')
    ax.set_xlabel('Time') 
    ax.set_ylabel('Exposure Rate [R/hr]')
    ax.set_title('Exposure Rate vs Time for a Single Dose Monitor')
    ax.legend()
    plt.show()

  def plotDistanceError(self, trueX, trueY):

    #the first couple of measurements are not used to estimate the position
    skip = 6

    #calculate the distance between the true position and the predicted position
    distance = np.sqrt(abs((np.array(self.measuredX) - trueX)**2 + (np.array(self.measuredY) - trueY)**2))
    posUncertainty = np.array(self.posUncertainty)
    plt.errorbar(self.predictionTimes[skip:self.updateIdx], distance[skip:self.updateIdx]*1000, yerr=posUncertainty[skip:self.updateIdx], fmt='o', color='k')
    plt.plot(self.predictionTimes[skip:self.updateIdx], np.zeros(len(self.predictionTimes[skip:self.updateIdx])), 'k--')
    plt.xlabel('Time')
    plt.ylabel('Distance from True Position (m)')
    plt.title('Distance from True Position')
    plt.show()

  def update(self, doseMeasurement, doseSigma, differentialToFs, time):
    #if this is the first measurement, set the initial state to the measurement
    if self.updateIdx == 0:
      self.doseRateFilter.x = np.array([doseMeasurement, 1])

    self.predictionTimes[self.updateIdx] = time

    #update the dose rate filter
    self.doseRateFilter.R = np.array([[doseSigma]])
    self.doseRateFilter.predict()
    self.doseRateFilter.update(doseMeasurement)
    self.estimatedDoseRateMeasurements[self.updateIdx] = self.doseRateFilter.x[0]
    try:
      self.estimatedDoseRateUncertainties[self.updateIdx] = np.sqrt(np.abs(self.doseRateFilter.P[0,0]))
    except:
      self.estimatedDoseRateUncertainties[self.updateIdx] = 0

    #update the time of flight filter
    self.timeOfFlightFilter.predict()

    #not all gateways may have reported
    gatewayIDs = differentialToFs.keys()
    dToFs = np.zeros(len(self.gateways))
    for id in gatewayIDs:
      dToFs[id] = differentialToFs[id]

    #for any differential time of flight measurements that are zero, set the corresponding time of flight to the previous value
    for i in range(len(dToFs)):
      if dToFs[i] == 0:
        dToFs[i] = self.timeOfFlightFilter.x[i]

    self.timeOfFlightFilter.update(dToFs)
    dToFs = self.timeOfFlightFilter.x
     
    if self.updateIdx > 0:
      initial_guess = [self.measuredX[self.updateIdx-1], self.measuredY[self.updateIdx-1], self.measuredR]
      result = minimize(diffToFerror, initial_guess, args=(dToFs, self.gateways), method='SLSQP', options={'ftol': 1e-6, 'disp': False})
    else:
      initial_guess = [0, 0, 1]
      result = minimize(diffToFerror, initial_guess, args=(dToFs, self.gateways), method='SLSQP', options={'ftol': 1e-6, 'disp': False})
  
    measuredX, measuredY, self.measuredR = result.x
    
    self.measuredX[self.updateIdx] = measuredX
    self.measuredY[self.updateIdx] = measuredY

    #use the latest ToF filter uncertainty to estimate the uncertainty in the position
    timeUncertainties = np.sqrt(abs(np.diag(self.timeOfFlightFilter.P)))
    posUncertainty = np.sqrt(abs(np.sum(timeUncertainties**2)))*3e2
    self.posUncertainty[self.updateIdx] = posUncertainty

    self.updateIdx += 1
  
  def getFeatures(self, idx):
    return np.array([self.measuredX[:idx], self.measuredY[:idx], self.posUncertainty[:idx], self.estimatedDoseRateMeasurements[:idx], self.estimatedDoseRateUncertainties[:idx]])

class Gateway:
  def __init__(self, x, y, id):
    self.x = x
    self.y = y
    self.id = id

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