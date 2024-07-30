import SCRAM_field
import numpy as np

class SimDetector(SCRAM_field.Detector):
  def __init__(self, x, y, simRadField, id, nGateways=4, positionMeasurementNoise=0.2, positionProcessVariance=1e-6, expectedDecayRate=1, 
               decayProcessVariance=0.1, detMeasurementUncertainty=0.2, minimumNoise=0.2, minimumNoiseUncertainty=0.01, ToFvariance=100):
    super().__init__(id, positionMeasurementNoise, positionProcessVariance, expectedDecayRate, decayProcessVariance, nGateways)
    self.trueX = x
    self.trueY = y
    self.simRadField = simRadField
    self.minimumNoise = minimumNoise
    self.ToFvariance = ToFvariance
    self.minimumNoiseUncertainty = minimumNoiseUncertainty
    self.trueDoseRates = []
    self.detMeasurementUncertainty = detMeasurementUncertainty
  
  def getTrueDoseRate(self, time):
    return self.simRadField.getDoseRate(self.trueX, self.trueY, time)
  
  def getDoseRateMeasurement(self, time):
    trueDoseRate = self.getTrueDoseRate(time)

    #add noise to the true dose rate
    doseMeasurement = trueDoseRate + np.random.normal(0, trueDoseRate*self.detMeasurementUncertainty ) + self.minimumNoise*np.random.normal(0, self.minimumNoiseUncertainty)
    #ensure the measurement is positive
    if doseMeasurement < 0:
      doseMeasurement = -doseMeasurement
    return doseMeasurement
  
  def getDifferentialTimeOfFlights(self, gateways):
    #calculate the time of flight to each gateway in ns, then add noise
    tofs = []

    for gateway in gateways:
      distance = np.sqrt((self.trueX - gateway.x)**2 + (self.trueY - gateway.y)**2) * 1000

      tof = distance / 299792458 * 1e9 #ns
      tof += np.random.normal(0, self.ToFvariance)
      tofs.append(tof)
    return tofs - np.min(tofs)
  
  def updateSim(self, time, gateways):
    self.trueDoseRates.append(self.getTrueDoseRate(time))
    doseMeasurement = self.getDoseRateMeasurement(time)
    doseSigma = doseMeasurement * self.detMeasurementUncertainty

    #generate time of flight measurements
    dToFs = self.getDifferentialTimeOfFlights(gateways)
    self.update(doseMeasurement, doseSigma, dToFs, time, gateways)
