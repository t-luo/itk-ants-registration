import ants

def affine_registration():
    """Affinely register each image to the fixed image (21)"""
    fixed_image = ants.image_read("../KKI2009-ALL-MPRAGE/KKI2009-21-MPRAGE.nii.gz")

    for i in range(1, 21):
        moving_image = ants.image_read("../KKI2009-ALL-MPRAGE/KKI2009-" + str(i) + "-MPRAGE.nii.gz")
        after_reg = ants.registration(fixed=fixed_image,
                                    moving=moving_image,
                                    type_of_transform='Affine',
                                    aff_metric='meansquares')
        ants.image_write(after_reg["warpedmovout"],
                        "../AffineReg/AntsAffineReg-" + str(i) + "-to-21.nii.gz")

if __name__ == '__main__':
    affine_registration()
