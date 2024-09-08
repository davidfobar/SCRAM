import SCRAM_field
import numpy as np

class SimDetector(SCRAM_field.Detector):
  def __init__(self, x, y, trueDoseRates, id, gateways, timesteps, positionMeasurementNoise=0.2, positionProcessVariance=1e-6, expectedDecayRate=1, 
               decayProcessVariance=0.1, detMeasurementUncertainty=0.2, minimumNoise=0.2, minimumNoiseUncertainty=0.01, ToFvariance=100):
    super().__init__(id, gateways, positionMeasurementNoise, positionProcessVariance, expectedDecayRate, decayProcessVariance)
    self.trueX = x
    self.trueY = y
    self.ToFvariance = ToFvariance
    self.trueDoseRates = trueDoseRates
    self.doseMeasurements = trueDoseRates + np.random.normal(0, trueDoseRates*detMeasurementUncertainty) + minimumNoise*np.random.normal(0, minimumNoiseUncertainty, len(trueDoseRates))
    self.detMeasurementUncertainty = detMeasurementUncertainty
    self.timesteps = timesteps
  
  def updateSim(self, time, timeIdx, gateways):
    doseMeasurement = self.doseMeasurements[timeIdx]
    doseSigma = doseMeasurement * self.detMeasurementUncertainty

    ToFs = np.zeros(len(gateways))
    for g, gateway in enumerate(gateways):
      distance = np.sqrt(abs((self.trueX - gateway.x)**2 + (self.trueY - gateway.y)**2)) * 1000 #km to m
      ToF = distance / 299792458 * 1e9 #ns
      ToF += np.random.normal(0, self.ToFvariance)
      ToFs[g] = ToF

    dToFs = {}
    minToF = np.min(ToFs)
    for ToF, gateway in zip(ToFs, gateways):
      dToFs[gateway.id] = ToF - minToF

    self.update(doseMeasurement, doseSigma, dToFs, time)

  def mp(self):
    for t, time in enumerate(self.timesteps):
      self.updateSim(time, t, self.gateways)