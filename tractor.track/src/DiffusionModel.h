#ifndef _DIFFUSION_MODEL_H_
#define _DIFFUSION_MODEL_H_

#include "Space.h"
#include "Grid.h"
#include "NiftiImage.h"

class DiffusionModel : public Griddable3D
{
protected:
    Grid<3> grid;
    
public:
    virtual ~DiffusionModel () {}
    
    virtual Space<3>::Vector sampleDirection (const Space<3>::Point &point, const Space<3>::Vector &referenceDirection) const
    {
        return Space<3>::zeroVector();
    }
    
    Grid<3> getGrid3D () const { return grid; }
};

class BedpostModel : public DiffusionModel
{
private:
    std::vector<Array<float>*> avf, theta, phi;
    int nCompartments;
    int nSamples;
    float avfThreshold;
    
public:
    BedpostModel () { nCompartments = 0; nSamples = 0; }
    BedpostModel (const std::vector<std::string> &avfFiles, const std::vector<std::string> &thetaFiles, const std::vector<std::string> &phiFiles)
    {
        if (avfFiles.size() == 0)
            throw std::invalid_argument("Vectors of BEDPOSTX filenames should not have length zero");
        if (avfFiles.size() != thetaFiles.size() || thetaFiles.size() != phiFiles.size())
            throw std::invalid_argument("Vectors of BEDPOSTX filenames should all have equal length");
        
        nCompartments = avfFiles.size();
        avf.resize(nCompartments);
        theta.resize(nCompartments);
        phi.resize(nCompartments);
        
        grid = NiftiImage(avfFiles[0],false).getGrid3D();
        
        for (int i=0; i<nCompartments; i++)
        {
            avf[i] = NiftiImage(avfFiles[i]).getData<float>();
            theta[i] = NiftiImage(thetaFiles[i]).getData<float>();
            phi[i] = NiftiImage(phiFiles[i]).getData<float>();
        }
        
        const std::vector<int> &avfDims = avf[0]->getDimensions();
        nSamples = avfDims[3];
    }
    
    ~BedpostModel ()
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
    
    Space<3>::Vector sampleDirection (const Space<3>::Point &point, const Space<3>::Vector &referenceDirection) const;
};

#endif
