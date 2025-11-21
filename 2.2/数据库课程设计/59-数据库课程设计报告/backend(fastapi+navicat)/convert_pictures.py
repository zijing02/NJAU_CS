from PIL import Image
import base64
import io
 
# Open an image file
with Image.open(r"D:\dongzijing\桌面\product_picture\01.png") as img:
    # Convert image to bytes
    image_format = img.format
    byte_arr = io.BytesIO()
    img.save(byte_arr, format=image_format)
    img_binary = byte_arr.getvalue()

# show bytes
# with open('convert_binary.txt', 'wb') as f:
#     f.write(img_binary)
# print(img_binary)

# Convert bytes to base64
img_base64 = base64.b64encode(img_binary)
# print(img_base64)

# Decode to ASCII string
img_base64_string = img_base64.decode('utf-8')
with open('convert_base64.txt', 'w') as f:
    f.write(img_base64_string)
# print(img_base64_string)

# Assuming img_binary is your binary image data
# Convert bytes to image
image = Image.open(io.BytesIO(img_binary))
 
# Display image
# image.show()
  


# 以二进制写入模式打开文件  
# with open(r'D:\dongzijing\桌面\二进制码.txt', 'wb') as f:  
#     f.write(img_binary)

#想文件存入base64码
# file_path = r"D:\dongzijing\桌面\二进制码.txt"  
# with open(file_path, 'a', encoding='utf-8') as f:  
#     f.write(img_base64_string)  