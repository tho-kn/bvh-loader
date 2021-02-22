#include "BVHReader.h"

class BVHViewer {
public:
	BVHViewer(std::vector<std::unique_ptr<Segment>> roots, Motion motion, int channels);

    void loadFrame(int frame);

	void draw();
    int frameSize();

private:
    std::vector<std::unique_ptr<Segment>> root;

    Motion motion;
    int channels;
};