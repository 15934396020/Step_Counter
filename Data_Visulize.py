import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import pathlib

# Load data from .csv table
ori_data = pd.read_csv('Original_Data.csv', encoding='gb18030')
fil_data = pd.read_csv('Filtered_Data.csv', encoding='gb18030')

# Original Data
ori_x = np.array(ori_data['X-Accelerate(m^2/s)'][:15000])
ori_y = np.array(ori_data['Y-Accelerate(m^2/s)'][:15000])
ori_z = np.array(ori_data['Z-Accelerate(m^2/s)'][:15000])
# Filtered Data
fil_x = np.array(fil_data['X-Accelerate(m^2/s)'][:15000])
fil_y = np.array(fil_data['Y-Accelerate(m^2/s)'][:15000])
fil_z = np.array(fil_data['Z-Accelerate(m^2/s)'][:15000])
# Steps
steps = np.array(ori_data['Y-Accelerate(m^2/s)'][15000:])

for i in range(15000):
    ori_x[i] = eval(ori_x[i])
    fil_x[i] = eval(fil_x[i])
    
# Font Family Set
plt.rcParams['font.sans-serif']=['SimHei']
plt.rcParams['axes.unicode_minus']=False
f = plt.figure(dpi=300,
              figsize=(3.5, 2.5))
# Number of Samples We Selected to Display
display_n = 150
# Public Horizontal Data
X = np.array([a for a in range(1, display_n+1)])

# Make sure the output exists
output = pathlib.Path('output')
if not output.exists():
    output.mkdir()
    
# x plot
plt.plot(X, ori_x[:display_n], color='#e73847', label='X-Axis')
# y plot
plt.plot(X, ori_y[:display_n], color='#a8d8db', label='Y-Axis')
# z plot
plt.plot(X, ori_z[:display_n], color='#457b9d', label='Z-Axis')
# Loc Show
plt.legend(loc="upper right")
# Title Set
plt.title("Generated Data Display")
# Axis Title
plt.xlabel("Sample Counts")
plt.ylabel("Deflection Accelerate(m^2/s)")
# Save the figure to local
plt.savefig('output/Original_Data.png')
# Display the Figure
plt.show()

# x plot
plt.plot(X, fil_x[:display_n], color='#e73847', label='X-Axis')
# y plot
plt.plot(X, fil_y[:display_n], color='#a8d8db', label='Y-Axis')
# z plot
plt.plot(X, fil_z[:display_n], color='#457b9d', label='Z-Axis')
# Loc Show
plt.legend(loc="upper right")
# Title Set
plt.title("Filtered Data Display")
# Axis Title
plt.xlabel("Sample Counts")
plt.ylabel("Deflection Angle(°)")
# Save the figure to local
plt.savefig('output/Filtered_Data.png')
# Display the Figure
plt.show()

# Load Tabel
thresholds_table = pd.read_csv('Thresholds.csv', encoding='gb18030')['Thresholds']
# Extent it
thresholds = []
for x in thresholds_table:
    for i in range(50):
        thresholds.append(x)
thresholds = np.array(thresholds)
# Primary Axis
primary_axis = pd.read_csv('Thresholds.csv', encoding='gb18030')['Primary Axis'][0]

# Original
plt.plot(X, ori_x[:display_n], color='#e73847', label='Original')
# Filtered
plt.plot(X, fil_x[:display_n], color='#1d3557', label='Filtered')
# Threshold
if primary_axis == 0:
    plt.plot(X, thresholds[:display_n], color='#FEFE00', label='Thresholds')
# Loc Show
plt.legend(loc="upper right")
# Title Set
plt.title("Compare On X-Axis")
# Axis Title
plt.xlabel("Sample Counts")
plt.ylabel("Deflection Accelerate(m^2/s)")
# Save the figure to local
plt.savefig('output/Filtered_Data_X.png')
# Display the Figure
plt.show()

# Original
plt.plot(X, ori_y[:display_n], color='#a8d8db', label='Original')
# Filtered
plt.plot(X, fil_y[:display_n], color='#1d3557', label='Filtered')
# Threshold
if primary_axis == 1:
    plt.plot(X, thresholds[:display_n], color='#FEFE00', label='Thresholds')
# Loc Show
plt.legend(loc="upper right")
# Title Set
plt.title("Compare On Y-Axis")
# Axis Title
plt.xlabel("Sample Counts")
plt.ylabel("Deflection Accelerate(m^2/s)")
# Save the figure to local
plt.savefig('output/Filtered_Data_Y.png')
# Display the Figure
plt.show()

# Original
plt.plot(X, ori_z[:display_n], color='#457b9d', label='Original')
# Filtered
plt.plot(X, fil_z[:display_n], color='#1d3557', label='Filtered')
# Threshold
if primary_axis == 2:
    plt.plot(X, thresholds[:display_n], color='#FEFE00', label='Thresholds')
# Loc Show
plt.legend(loc="upper right")
# Title Set
plt.title("Compare On Z-Axis")
# Axis Title
plt.xlabel("Sample Counts")
plt.ylabel("Deflection Accelerate(m^2/s)")
# Save the figure to local
plt.savefig('output/Filtered_Data_Z.png')
# Display the Figure
plt.show()
