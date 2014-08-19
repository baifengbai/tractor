#ifndef _DIFFUSION_DATA_SOURCE_H_
#define _DIFFUSION_DATA_SOURCE_H_

#include "Space.h"
#include "NiftiImage.h"

class DiffusionDataSource
{
public:
    virtual ~DiffusionDataSource () {}
    
    virtual Space<3>::Vector sampleDirection (const Space<3>::Point &point, const Space<3>::Vector &referenceDirection)
    {
        return Space<3>::zeroVector();
    }
};

class BedpostDataSource : public DiffusionDataSource
{
private:
    std::vector<Array<float>*> avf, theta, phi;
    std::vector<int> imageDims;
    int nCompartments;
    int nSamples;
    float avfThreshold;
    
public:
    BedpostDataSource () { nCompartments = 0; nSamples = 0; }
    BedpostDataSource (const std::vector<std::string> &avfFiles, const std::vector<std::string> &thetaFiles, const std::vector<std::string> &phiFiles)
    {
        if (avfFiles.size() == 0)
            throw std::invalid_argument("Vectors of BEDPOSTX filenames should not have length zero");
        if (avfFiles.size() != thetaFiles.size() || thetaFiles.size() != phiFiles.size())
            throw std::invalid_argument("Vectors of BEDPOSTX filenames should all have equal length");
        
        nCompartments = avfFiles.size();
        avf.resize(nCompartments);
        theta.resize(nCompartments);
        phi.resize(nCompartments);
        
        imageDims = NiftiImage(avfFiles[0],false).getDimensions();
        
        for (int i=0; i<nCompartments; i++)
        {
            avf[i] = NiftiImage(avfFiles[i]).getData<float>();
            theta[i] = NiftiImage(thetaFiles[i]).getData<float>();
            phi[i] = NiftiImage(phiFiles[i]).getData<float>();
        }
        
        const std::vector<int> &imageDims = avf[0]->getDimensions();
        nSamples = imageDims[3];
    }
    
    ~BedpostDataSource ()
    {
        for (int i=0; i<nCompartments; i++)
        {
            delete avf[i];
            delete theta[i];
            delete phi[i];
        }
    }
    
    int getNCompartments () const { return nCompartments; }
    int getNSamples () const { return nSamples; }
    float getAvfThreshold () const { return avfThreshold; }
    
    void setAvfThreshold (const float avfThreshold) { this->avfThreshold = avfThreshold; }
    
    Space<3>::Vector sampleDirection (const Space<3>::Point &point, const Space<3>::Vector &referenceDirection);
};

#endif
