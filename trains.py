import matplotlib.pyplot as plt

# Data
train_ids = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23]
transport_times = [38.5297, 45.1706, 16.7798, 50.6134, 41.7215, 37.1441, 39.6783, 78.6042, 30.5296, 62.0063, 29.7553, 28.3440, 0.8785, 38.1176, 67.6868, 71.4942, 101.1217, 27.5473, 49.8695, 49.2331, 28.1235, 34.6457, 4.6176]
delay_times = [0.0000, 34.5685, 0.0000, 34.5685, 0.0000, 17.5812, 0.0000, 23.7765, 0.0000, 23.7765, 0.0000, 23.7765, 0.0000, 9.4412, 64.1952, 9.4412, 64.1952, 9.4412, 7.8114, 9.4412, 0.5345, 9.4412, 0.5345]
total_times = [38.5297, 10.6022, 16.7798, 16.0449, 41.7215, 19.5629, 39.6783, 54.8277, 30.5296, 38.2298, 29.7553, 4.5675, 0.8785, 28.6764, 3.4915, 62.0531, 36.9265, 18.1061, 42.0580, 39.7919, 27.5890, 25.2046, 4.0831]

# Plotting
fig, ax = plt.subplots(figsize=(10, 6))
bar_width = 0.25

bar1 = ax.bar(train_ids, transport_times, width=bar_width, label='Transport Time')
bar2 = ax.bar([id + bar_width for id in train_ids], delay_times, width=bar_width, label='Delay Time')
bar3 = ax.bar([id + 2 * bar_width for id in train_ids], total_times, width=bar_width, label='Total Time')

# Adding labels and title
ax.set_xlabel('Train ID')
ax.set_ylabel('Time')
ax.set_title('Train Transport Simulation')
ax.legend()

# Show the plot
plt.show()
