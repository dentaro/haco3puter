import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

PImage img;
ArrayList<Integer> colorList = new ArrayList<Integer>();

void setup() {
  //size(128, 128);
  size(256, 256);
  img = loadImage("map.png");
  processImage();
}

void draw() {
  image(img, 0, 0);
}

void processImage() {
  // Create a PrintWriter to write to a CSV file
  PrintWriter output = createWriter("output.csv");

  // Define the mapping from original color index to the specified numbers
  //int[] colorMapping = {45, 43, 23, 44, 42, 18, 54, 30, 22, 29, 22, 55, 46, 64, 79, 80};
  //Object[] colorMapping = { 0,  1,  2,  3,  4, 'i',  5,  6, 'b', 'n',  7, 's',  8, 'h', 'b', 'c'};
  Object[] colorMapping = { 0,  1,  7,  3,  4,  5,  6,  6, 4, 0,  2, 7,  7, 7, 4, 4};

  // Loop through each pixel in the image
  for (int y = 0; y < img.height; y++) {
    int currentColor = img.get(0, y);
    int count = 1;

    // Loop through each column in the row
    for (int x = 1; x < img.width; x++) {
      int pixelColor = img.get(x, y);

      if (currentColor == pixelColor) {
        // Same color, increment the count
        count++;
      } else {
        // Different color, write the data and reset the count
        output.print(getColorMapping(colorMapping, getColorIndex(currentColor)));
        output.print(",");
        output.print(count);
        output.print(",");

        currentColor = pixelColor;
        count = 1;
      }
    }

    // Write the last color data
    output.print(getColorMapping(colorMapping, getColorIndex(currentColor)));
    output.print(",");
    output.print(count);

    // Move to the next line in the CSV file
    output.println();
  }

  // Close the PrintWriter
  output.flush();
  output.close();

  // Print a message to indicate that the CSV file has been created
  println("CSV file created: output.csv");
// Output compressed data to a binary file
DataOutputStream compressedOutput = null;
try {
  compressedOutput = new DataOutputStream(new FileOutputStream("w.bin"));

  String[] lines = loadStrings("output.csv");
  int totalLines = lines.length;

  for (int lineIndex = 0; lineIndex < totalLines; lineIndex++) {
    String line = lines[lineIndex];
    String[] tokens = splitTokens(line, ",");
    int numTokens = tokens.length;

    for (int i = 0; i < numTokens; i += 2) {
      int evenPart = Integer.parseInt(tokens[i + 1]);  // 5ビットの繰り返し回数
      int oddPart = Integer.parseInt(tokens[i]);       // 3ビットのスプライト番号

      // Split count if it exceeds 31
      while (evenPart > 31) {
        int compressedData = (oddPart << 5) | 0b11111;
        compressedOutput.write((byte) compressedData);
        evenPart -= 31;
      }

      int compressedData = (oddPart << 5) | (evenPart & 0b11111);

      // Write the compressed data to the binary file
      compressedOutput.write((byte) compressedData);
    }

    // Add a newline as an indicator for the end of the compressed data, except for the last line
    if (lineIndex < totalLines - 1) {
      compressedOutput.write(0b01111111);
      //compressedOutput.write(0x0A);  // Add newline character
      //compressedOutput.write('\n');  // Add newline character
      
    }
  }

  println("Binary file created: compressed_data.bin");
} catch (IOException e) {
  e.printStackTrace();
  println("Error: Unable to write to compressed_data.bin");
} finally {
  if (compressedOutput != null) {
    try {
      compressedOutput.close();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
}

Object getColorMapping(Object[] mapping, int index) {
  return mapping[index];
}

int getColorIndex(int col) {
  // Check if the color is already in the colorList
  int existingColorIndex = colorList.indexOf(col);

  // If the color is not in the list, add it
  if (existingColorIndex == -1) {
    existingColorIndex = colorList.size();
    colorList.add(col);
  }

  return existingColorIndex;
}
