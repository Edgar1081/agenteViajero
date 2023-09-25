import xml.etree.ElementTree as ET

# Define the path to your existing SVG file
input_svg_file = './src/plot/world.svg'

# Parse the existing SVG file
tree = ET.parse(input_svg_file)
root = tree.getroot()

# Define the coordinates for your lines
lines = [
    ((0, 0), (100, 100)),  # Example line from (0, 0) to (100, 100)
    ((200, 200), (300, 300)),  # Example line from (200, 200) to (300, 300)
]

# Create and add line elements to the SVG
for line in lines:
    start, end = line
    line_element = ET.Element('line')
    line_element.set('x1', str(start[0]))
    line_element.set('y1', str(start[1]))
    line_element.set('x2', str(end[0]))
    line_element.set('y2', str(end[1]))
    line_element.set('stroke', 'red')  # You can customize the line color and style here
    root.append(line_element)

# Save the modified SVG to a new file
output_svg_file = 'output_map.svg'
tree.write(output_svg_file)

print(f"Lines added to {output_svg_file}")
