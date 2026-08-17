#include <vocalmelody/pitch/IPitchEstimator.h>

#include <type_traits>

static_assert(std::has_virtual_destructor_v<vocalmelody::pitch::IPitchEstimator>);

int main() { return 0; }
