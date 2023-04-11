#ifndef ASNSEQUENCE_H
#define ASNSEQUENCE_H

#include "sequenceof.h"
//#include "osnmp/variant.h"

namespace osnmp {
    //    using Sequence = SequenceOf <Variant>;
    using Sequence = SequenceOf <RawData>;  // Variant
}

#endif // SEQUENCE_H
