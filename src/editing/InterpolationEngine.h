#pragma once

#include "../maps/MapDefinition.h"
#include "../binary/BinaryFile.h"

namespace WinMMM10 {

enum class InterpolationType {
    Linear,
    Cubic,
    Spline
};

class InterpolationEngine {
public:
    InterpolationEngine(BinaryFile* file);
    ~InterpolationEngine() = default;
    
    bool interpolateMap(const MapDefinition& map, InterpolationType type = InterpolationType::Linear);
    bool smoothMap(const MapDefinition& map, int kernelSize = 3);
    bool interpolateRegion(const MapDefinition& map, size_t startRow, size_t startCol,
                          size_t endRow, size_t endCol, InterpolationType type = InterpolationType::Linear);

private:
    double readMapValue(const MapDefinition& map, size_t row, size_t col) const;
    void writeMapValue(const MapDefinition& map, size_t row, size_t col, double value);
    double linearInterpolate(double x0, double y0, double x1, double y1, double x) const;
    double cubicInterpolate(double p0, double p1, double p2, double p3, double t) const;
    
    BinaryFile* m_binaryFile;
};

} // namespace WinMMM10

