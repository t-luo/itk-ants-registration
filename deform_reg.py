import ants

def deformable_registration():
    """Deformably register each image to the fixed image (21) with Demons algo"""
    fixed_image = ants.image_read("../AffineReg/ants-affine-template.nii.gz")
    iterations = [25, 50]
    for iteration in iterations:
        for i in range(1, 21):
            moving_image = ants.image_read("../AffineReg/AntsAffineReg-" + str(i) +"-to21.nii.gzz")
            after_reg = ants.registration(fixed=fixed_image,
                                        moving=moving_image,
                                        type_of_transform='SyN',
                                        aff_metric='meansquares',
                                        syn_metric='demons',
                                        reg_iterations=[iteration])
            ants.image_write(after_reg["warpedmovout"],
                            "../DeformReg/AntsDeformReg-" + str(i) + "-" + str(iteration) + ".nii.gz")

if __name__ == '__main__':
    deformable_registration()
