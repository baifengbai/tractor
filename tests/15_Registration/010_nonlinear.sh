#@desc Checking nonlinear registration
cp data/session-12dir/tractor/transforms/diffusion2mni.Rdata tmp/transform.Rdata
${TRACTOR} reg-nonlinear data/session-12dir/tractor/diffusion/refb0 ../share/mni/brain TransformationName:tmp/transform Levels:2 EstimateOnly:true
${TRACTOR} peek tmp/transform
rm -f tmp/transform*