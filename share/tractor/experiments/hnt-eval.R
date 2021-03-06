#@desc Evaluate a series of candidate tracts for similarity to a reference tract. The specified TractName must match that passed to the "hnt-ref" experiment used to generate the reference tract. If the seed point is omitted then the standard space seed point associated with the reference tract will be used to establish the neighbourhood centre point. Any candidate seed point with anisotropy lower than AnisotropyThreshold will be ignored.
#@args session directory, [seed point]
#@deprecation HNT is deprecated in favour of PNT. See <http://www.tractor-mri.org.uk> for details.

library(tractor.reg)
library(tractor.session)
library(tractor.nt)

runExperiment <- function ()
{
    tractName <- getConfigVariable("TractName", NULL, "character", errorIfMissing=TRUE)
    searchWidth <- getConfigVariable("SearchWidth", 1)
    faThreshold <- getConfigVariable("AnisotropyThreshold", 0.2)
    nStreamlines <- getConfigVariable("Streamlines", 1000)
    resultsName <- getConfigVariable("ResultsName", "results")
    
    reference <- getNTResource("reference", "hnt", list(tractName=tractName))
    
    session <- attachMriSession(Arguments[1])
    if (nArguments() > 1)
        seed <- splitAndConvertString(Arguments[-1], ",", "numeric", fixed=TRUE, errorIfInvalid=TRUE)
    else
        seed <- transformPointsToSpace(reference$getStandardSpaceSeedPoint(), session, "diffusion", oldSpace="mni", pointType=reference$getSeedUnit(), outputVoxel=TRUE, nearest=TRUE)
    
    result <- runNeighbourhoodTractography(session, seed, reference$getTract(), faThreshold, searchWidth, nStreamlines=nStreamlines)
    
    resultsObject <- HeuristicNTResults$new(list(result), Arguments[1])
    if (isValidAs(Sys.getenv("TRACTOR_PLOUGH_ID"), "integer"))
        resultsName <- paste(ensureFileSuffix(resultsName,NULL,strip="Rdata"), Sys.getenv("TRACTOR_PLOUGH_ID"), sep=".")
    writeNTResource(resultsObject, "results", "hnt", list(resultsName=resultsName))
}
