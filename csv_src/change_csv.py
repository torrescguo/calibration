import pandas as pd
import numpy as np

csv_path = 'imu0.csv'
df = pd.read_csv(csv_path)

# -----edit columns and index----- #
df.columns = ['timestamp', 'omega_x', 'omega_y', 'omega_z', 'alpha_x', 'alpha_y', 'alpha_z']
df.index = range(df.shape[0])
print(df.index)

# df.index = [i+1 for i in range(df.shape[0])]

# -----columns operations----- #
# Y = df['timestamp']
# print(Y) 
print("---------------------------------")
# df['omega_x'] = [0 for i in range(df.shape[0])]        # add
# df['omega_y'] = [0 for i in range(df.shape[0])]
# print(df)
# df.drop(columns='timestamp', inplace=True)                # delete
# print(df)

# 角速度逆时针为正，顺时针为负，模组坐标系下
omega_x_temp = np.array(df['omega_x'])
# df['omega_x'] = df['omega_y']#[6 for i in range(df.shape[0])]      # correct --(1)
# df['omega_y'] = -omega_x_temp
# df['omega_z'] = df['omega_z']

alpha_x_temp = np.array(df['alpha_x'])
# df['alpha_x'] = df['alpha_y']
# #[6 for i in range(df.shape[0])]      # correct --(1)
# df['alpha_y'] = -alpha_x_temp
# df['alpha_z'] = df['alpha_z']

df['alpha_x'] = -df['alpha_x'] 
#[6 for i in range(df.shape[0])]      # correct --(1)
df['alpha_y'] = -df['alpha_y']
df['alpha_z'] = -df['alpha_z']

# -----edit index again after rows operations!!!----- #
# df.index = range(df.shape[0])

# -----save dataframe as csv----- #
csv_save_path='./imu0_fixed.csv'
df.to_csv(csv_save_path, sep=',', index=False, header=True)

print(df)

