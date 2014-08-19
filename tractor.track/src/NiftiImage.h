#ifndef _NIFTI_IMAGE_H_
#define _NIFTI_IMAGE_H_

#include <RcppArmadillo.h>

#include "nifti1_io.h"

#include "Array.h"

class NiftiImage
{
private:
    nifti_image *info;
    std::vector<int> dims;
    std::vector<float> voxelDims;
    arma::fmat44 xform;
    
    template <typename SourceType, typename TargetType> static TargetType convertValue (SourceType value) { return static_cast<TargetType>(value); }
    template <typename SourceType, typename TargetType> Array<TargetType> * convertToArray ();
    
public:
    NiftiImage () {}
    NiftiImage (const std::string &fileName, const bool readData = true)
    {
        info = nifti_image_read(fileName.c_str(), static_cast<int>(readData));
        if (info == NULL)
            throw std::runtime_error("Cannot read NIfTI file " + fileName);
        else
        {
            dims.resize(info->ndim);
            voxelDims.resize(info->ndim);
            for (int i=0; i<info->ndim; i++)
            {
                dims[i] = info->dim[i+1];
                voxelDims[i] = fabs(info->pixdim[i+1]);
            }
            
            if (info->qform_code > 0)
                xform = arma::fmat44(*(info->qto_xyz.m));
            else
                xform = arma::fmat44(*(info->sto_xyz.m));
        }
    }
    
    ~NiftiImage ()
    {
        nifti_image_free(info);
    }
    
    const std::vector<int> & getDimensions () const { return dims; }
    const std::vector<float> & getVoxelDimensions () const { return voxelDims; }
    const arma::fmat44 & getXformMatrix () const { return xform; }
    
    template <typename DataType> Array<DataType> * getData () const;
    
    void dropData () { nifti_image_unload(info); }
};

#endif
