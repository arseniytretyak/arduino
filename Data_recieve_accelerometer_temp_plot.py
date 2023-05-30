import pandas as pd
import matplotlib.pyplot as plt
from Adafruit_IO import Client, Feed
from datetime import datetime, timedelta

# Define your Adafruit IO key and username
ADAFRUIT_IO_KEY = 'aio_IXBv19fDwO9HE3pD48TwSZFPw4rN'
ADAFRUIT_IO_USERNAME = 'arseniytretyak'

# Create an instance of the Adafruit IO client
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Define the name of your feed
FEED_NAME = 'lightbluearduino'

# Get the feed object from Adafruit IO
feed = aio.feeds(FEED_NAME)

# Get the data from the feed
data = aio.data(feed.key,max_results=None)

# Lists to hold t1 and t2 values
t1_values = []
t2_values = []
X_angle = []
Y_angle = []
timestamps = []

# Loop through each data point and extract t1 and t2 values
for datapoint in data:
    # Convert from hex to string and split into t1 and t2 values
    try:
        t1_str, t2_str, X_angle_str, Y_angle_str = bytes.fromhex(datapoint.value).decode('utf-8').split('/')
        t1 = float(t1_str)
        t2 = float(t2_str)
        X = float(X_angle_str)
        Y = float(Y_angle_str)
    except ValueError:
        print(f"Skipping data point with unexpected value format: {datapoint.value}")
        continue

    # Get timestamp and add t1 and t2 values to lists
    timestamp = datetime.strptime(datapoint.created_at, '%Y-%m-%dT%H:%M:%SZ')
    timestamp += timedelta(hours=2) # adjust for time difference
    timestamps.append(timestamp)
    t1_values.append(t1)
    t2_values.append(t2)
    X_angle.append(X)
    Y_angle.append(Y)

# Create a pandas DataFrame to store the data
df = pd.DataFrame({'t1': t1_values, 't2': t2_values,'X':X_angle,'Y':Y_angle,'timestamp': timestamps})

# Save the data as an Excel file
# df.to_excel('temperature_angle_data.xlsx', index=False)

# Create a figure with three subplots
fig, axs = plt.subplots(3, 1, figsize=(8, 10))

# Plot the first graph
axs[0].plot(timestamps, t1_values, label='t1')
axs[0].plot(timestamps, t2_values, label='t2')
axs[0].set_xlabel('Time')
axs[0].set_ylabel('Temperature (Celsius)')
axs[0].legend()

# Plot the second graph
axs[1].plot(timestamps, X_angle, label='X_angle')
axs[1].set_xlabel('Time')
axs[1].set_ylabel('X angle')
axs[1].legend()

# Plot the third graph
axs[2].plot(timestamps, Y_angle, label='Y_angle')
axs[2].set_xlabel('Time')
axs[2].set_ylabel('Y angle')
axs[2].legend()

# Display the plots
plt.show()
# Plot the data
plt.figure(1)
plt.plot(timestamps, t1_values, label='t1')
plt.plot(timestamps, t2_values, label='t2')
plt.xlabel('Time')
plt.ylabel('Temperature (Celsius)')
plt.legend()
plt.show()
# Plot the data
plt.figure(2)
plt.plot(timestamps, X_angle, label='X_angle')
plt.xlabel('Time')
plt.ylabel('X angle')
plt.legend()
plt.show()
# Plot the data
plt.figure(3)
plt.plot(timestamps, Y_angle, label='X_angle')
plt.xlabel('Time')
plt.ylabel('Y angle')
plt.legend()
plt.show()

