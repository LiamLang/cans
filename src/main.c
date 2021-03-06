/*
This program calculates the optimum way to build a hexagonal tower out of a large number of empty cans, or other cylindrical objects.
Layers are repeated so that all cans will be used.
Groups of three are added just below the top for a more pleasing result.
Results are displayed using text and primitive ASCII art.
Liam Lang <liammlang@gmail.com>
February 2016
*/

#include <stdio.h>
#include <math.h>

// Constants
#define maxCans 8000 // Maximum number of cans allowed
#define maxLayerSize 20 // Maximum cans along edge of hexagonal layer, should be set to at least cube root of maxCans

void printRepeatedly(char *msg, int times);

void main() {

	// Declare variables
	int cans = 0;
	int layerSize[maxLayerSize];
	int i = 0;
	int hexTowerHeight = 0;
	int layer[maxLayerSize * 3]; // to be safe
	int currentLayer = 0;
	int j = 0;
	int baseSize = 0;
	int spaces = 0;

	// Read in number of cans between 1 and maxCans
	while (cans < 1 || cans > maxCans) {
		printf("Please enter number of cans: ");
		scanf("%d", &cans);

		if (cans < 1 || cans > maxCans) {
			printf("Error, please enter a number between 1 and %d cans.\n", maxCans);
		}
	}

	printf("\n--------------------------------------------------");
	printf("\nIdeal tower of %d cans:", cans);
	printf("\n--------------------------------------------------\n");

	// Establish number of cans needed for each layer
	// Note a hexagonal pyramid of n layers contains n^3 cans
	for (i = 0; i < maxLayerSize; i++) {
		layerSize[i] = pow(i + 1, 3) - pow(i, 3);
	}

	// Establish largest hexagonal tower that can be made
	hexTowerHeight = pow(cans, 0.333334);

	// Get number of cans left over
	cans -= pow(hexTowerHeight, 3);

	// Build tower up layer by layer, starting from bottom
	// i tracks the side length of the current layer
	// layer[] tracks the side length of each layer of the tower
	i = hexTowerHeight;
	while (1) {

		if (i == 1) {

			// Groups of 3 are also added at the top, represented by a value of -1 in layer[]
			while (cans >= 3) {
				layer[currentLayer] = -1;
				cans -= 3;
				currentLayer++;
			}

			// Individual cans at top
			while (cans >= 1) {
				layer[currentLayer] = 1;
				cans -= 1;
				currentLayer++;
			}

			// Add top can
			layer[currentLayer] = i;
			currentLayer++;

		} else {

			// Add layer
			layer[currentLayer] = i;
			currentLayer++;

			// Add extra layers of same side length
			while (cans >= layerSize[i - 1]) {
				layer[currentLayer] = i;
				cans -= layerSize[i - 1];
				currentLayer++;
			}
		}

		// Move to smaller layer size, break if we have reached the top
		i--;
		if (i == 0) {
			break;
		}
	}

	// Get width of base (number of characters), so we know how many spaces to include before other layers in graphic representation
	if (layer[0] != -1) {
		// 2 * partially visible cans at side + cans in centre (2 chars each)
		baseSize = 2 * (layer[0] / 2) + 2 * layer[0];
	} else {
		// Always drawn as [[]]
		baseSize = 4;
	}

	// Go through layers from top to bottom, displaying them
	for(i = currentLayer - 1; i >= 0; i--) {

		// Annotations
		if (layer[i] > 1) {
			printf("\nHexagon, side length %d -->", layer[i]);
		} else if (layer[i] == 1) {
			printf("\nSingle can -->            ");
		} else if (layer[i] == -1) {
			printf("\nGroup of 3 -->            ");
		}

		if (layer[i] != -1) {
			// All layers except group of 3

			// Get number of spaces to use before showing cans
			// 8 minus length of int showing side length of layer, so all layers are in line even if annotations take up different amount of space
			spaces = 8 - floor (log10 (abs (layer[i]))) + 1;
			// Add half of difference between size of base and size of this layer
			spaces += (baseSize - (2 * (layer[i] / 2) + 2 * layer[i])) / 2;

			printRepeatedly(" ", spaces);

			// Draw cans
			// Partially visible cans in each hex layer take up sin(30) = 1/2 of the width of visible cans, on each side
			printRepeatedly("[", layer[i] / 2);
			printRepeatedly("[]", layer[i]);
			printRepeatedly("]", layer[i] / 2);

		} else {
			// Group of 3

			// Get number of spaces to use before showing cans
			spaces = 9 + (baseSize - 4) / 2;

			printRepeatedly(" ", spaces);

			// Draw group of 3
			printf("[[]]");
		}
	}

	// Line break and pause for user to see results
	printf("\n\n");
	system("pause");
}

// Print a string repeatedly
void printRepeatedly(char *msg, int times) {

	int i;
	for(i = 0; i < times; i++) {
		printf(msg);
	}
}