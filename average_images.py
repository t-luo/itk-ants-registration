import ants

def average_images():
    """Return the average of all images in list"""
    combined_image = ants.image_read("../DeformReg/DeformReg-1-50.nii.gz")

    for i in range(2, 22):
        image_name = "../DeformReg/DeformReg-" + str(i) + "-50.nii.gz"
        combined_image += ants.image_read(image_name)

    combined_image /= 21
    ants.image_write("ants_average_template.nii.gz")


if __name__ == '__main__':
    average_images()
