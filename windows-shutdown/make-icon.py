from PIL import Image
img = Image.open("./assets/shutdown-icon.png")
img.save("icon.ico", sizes=[(16,16), (32,32), (48,48),(64,64),(96,96),(96,96), (256,256)])