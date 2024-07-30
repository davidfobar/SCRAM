import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import pickle
from scipy.optimize import curve_fit

sourceDirectory = './delfic_test/'

def exponential_decay(x, a, b):
  return a * np.exp(-b * x)

#define a class to hold a radiation field that will be used by other classes
class RadiationField:
  def __init__(self, burst_lat, burst_long, burst_time, altitude):
    self.lat = burst_lat
    self.long = burst_long
    self.time = burst_time
    self.altitude = altitude
  
  def plotField(self):
    raise NotImplementedError("Subclasses must implement plotField method")

  def getDoseRate(self, x, y, time):
    raise NotImplementedError("Subclasses must implement calculate_intensity method")
  
class DelficField(RadiationField):
  def __init__(self, filename, plotInitial=False):
    with open(sourceDirectory + filename + f'/{filename}.pkl', 'rb') as f:
        self.fieldData = pickle.load(f)

    if plotInitial:
      self.plotField()

  def plotField(self, timestep=0, plotLimits=100):
    cmap = plt.cm.jet
    cmaplist = [cmap(i) for i in range(cmap.N)]
    cmaplist[0] = (1, 1, 1, 1.0)
    cmap = mcolors.LinearSegmentedColormap.from_list('Custom cmap', cmaplist, cmap.N)

    # Plotting

    # show the data
    plt.imshow(self.fieldData[timestep], origin='lower', cmap=cmap, vmin=0, vmax=np.max(self.fieldData), alpha=0.5)
    plt.colorbar(label='Fallout Exposure Rate')

    # add a 1km square grid to the plot, 10 units per km, 
    plt.grid(which='both', color='black', linestyle='-', linewidth=0.5)
    ax = plt.gca()
    ax.set_xticks(np.arange(0, self.fieldData[timestep].shape[1], 10))
    ax.set_yticks(np.arange(0, self.fieldData[timestep].shape[0], 10))
    ax.set_xticklabels(np.arange(0, self.fieldData[timestep].shape[1], 10)//10)
    ax.set_yticklabels(np.arange(0, self.fieldData[timestep].shape[0], 10)//10)

    plt.xlim(self.fieldData[timestep].shape[1]//2 - plotLimits/2, self.fieldData[timestep].shape[1]//2 + plotLimits/2)
    plt.ylim(self.fieldData[timestep].shape[0]//2 - plotLimits/2, self.fieldData[timestep].shape[0]//2 + plotLimits/2)
    plt.show()

  def getDoseRate(self, x: float, y: float, time: float): 
    #2d interpolation, x and y are floats with units of km, delfic is in 100m increments
    #timestep is an integer, 0 is the first timestep 1-hour post burst
    #returns the dose rate at the point x, y at the given time post burst
    #time is in hours, i.e. 0.5 is 30 minutes post burst

    x = x * 10
    y = y * 10

    x_lower = int(x)
    x_upper = x_lower + 1
    y_lower = int(y)
    y_upper = y_lower + 1

    #the dose rates are only for hour increments, so use the nearest 5 hours to fit an 
    #exponential decay curve to the data for each of the surrounding x,y coordinates, and then interpolate
    expTimeSteps = [int(time) - 2, int(time) - 1, int(time), int(time) + 1, int(time) + 2]

    #if any timesteps are less than 0, remove it
    if expTimeSteps[0] < 0:
      expTimeSteps = [t for t in expTimeSteps if t >= 0]
      numToAppend = 5 - len(expTimeSteps)
      for i in range(numToAppend):
        expTimeSteps.append(expTimeSteps[-1] + 1)

    #if any timesteps are greater than the number of timesteps, remove it
    if expTimeSteps[-1] >= len(self.fieldData):
      expTimeSteps = [t for t in expTimeSteps if t < len(self.fieldData)]
      numToAppend = 5 - len(expTimeSteps)
      for i in range(numToAppend):
        expTimeSteps.insert(0, expTimeSteps[0] - 1)

    expParams = [] #[upper left, upper right, lower left, lower right]
    for i in [x_lower, x_upper]:
      for j in [y_lower, y_upper]:
        data = []
        for t in expTimeSteps:
          data.append(self.fieldData[t][j][i])
        popt, pcov = curve_fit(exponential_decay, expTimeSteps, data, p0=[1, 0.1])
        expParams.append(popt)

    #get the dose rates at the four corners of the square
    dose_rate_lower_left = exponential_decay(time, expParams[2][0], expParams[2][1])
    dose_rate_lower_right = exponential_decay(time, expParams[3][0], expParams[3][1])
    dose_rate_upper_left = exponential_decay(time, expParams[0][0], expParams[0][1])
    dose_rate_upper_right = exponential_decay(time, expParams[1][0], expParams[1][1])

    #interpolate between the four corners
    x_fraction = x - x_lower
    y_fraction = y - y_lower

    dose_rate_lower = dose_rate_lower_left + (dose_rate_lower_right - dose_rate_lower_left) * x_fraction
    dose_rate_upper = dose_rate_upper_left + (dose_rate_upper_right - dose_rate_upper_left) * x_fraction

    dose_rate = dose_rate_lower + (dose_rate_upper - dose_rate_lower) * y_fraction

    return dose_rate  

class GaussianField(RadiationField):
  def __init__(self, burst_lat, burst_long, burst_time=0, altitude=0, sigma=1, decayConstant=1):
    super().__init__(burst_lat, burst_long, burst_time, altitude)
    self.sigma = sigma
    self.decayConstant = decayConstant

  def plotFields(self, time=0):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    x = np.linspace(-10, 10, 100)
    y = np.linspace(-10, 10, 100)
    x, y = np.meshgrid(x, y)
    z = np.exp(-((x**2 + y**2)/(2*self.sigma**2)))
    z = np.exp(-time * self.decayConstant)*z
    ax.plot_surface(x, y, z, cmap='viridis')
    ax.set_xlabel('X') 
    ax.set_ylabel('Y')
    ax.set_zlabel('Intensity')
    ax.set_zlim(0, 1)
    plt.show()

  def plotFields(self, times=[0,1,2]):
    #create a figure with subplots for each time
    fig = plt.figure()
    for t in times:
      ax = fig.add_subplot(1, len(times), t+1, projection='3d')
      x = np.linspace(-10, 10, 100)
      y = np.linspace(-10, 10, 100)
      x, y = np.meshgrid(x, y)
      z = np.exp(-((x**2 + y**2)/(2*self.sigma**2)))
      z = np.exp(-t * self.decayConstant)*z
      ax.plot_surface(x, y, z, cmap='viridis')
      ax.set_xlabel('X') 
      ax.set_ylabel('Y')
      ax.set_zlabel('Intensity')
      ax.set_zlim(0, 1)
      ax.set_title('Time = ' + str(t))
    plt.show()

  def getDoseRate(self, x, y, time):
    #calculate the distance between the burst and the point
    distance = np.sqrt(x**2 + y**2 + altitude**2)
    #calculate the time since the burst
    timeSinceBurst = time - self.time
    #calculate the intensity
    intensity = np.exp(-timeSinceBurst * self.decayConstant) * np.exp(-distance**2 / (2*self.sigma**2))
    return intensity