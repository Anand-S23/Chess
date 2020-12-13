from PIL import Image
import PIL

file = input("Enter image filename (without filetype): ")
im = PIL.Image.open(f"W:/Chess/data/{file}.png")
width, height = im.size
im = im.resize((int(width//1.5), int(height//1.5)))
im.save(f"{file}.png")
