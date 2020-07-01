import sys
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.colors as colors

# 0: white, 1: black, 2: blue, 3: green, 4: yellow, 5: red, 6: orange, 7: purple, 8: grey
cmap = colors.ListedColormap(['white', 'black', '#386cb0', '#7fc97f', 'yellow', 'red', 'orange', 'purple', 'grey'])

img_file = open(sys.argv[1], 'r').read()
img_file = img_file.split('\n')
xy = img_file[0].split(',')
x = int(xy[0])
y = int(xy[1])

img_matrix = []

for i in range(0, y):
	img_line = img_file[i+1]
	temp_line = []
	for j in range(0, x):
		temp_line.append(int(img_line[j:j+1]))
	img_matrix.append(temp_line)

matrix = np.array(img_matrix)

norm = colors.BoundaryNorm(np.arange(len(matrix)+1)-0.5, len(matrix))


plt.matshow(matrix, cmap=cmap, norm=norm)
plt.show()

