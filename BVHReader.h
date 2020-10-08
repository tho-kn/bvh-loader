#include "Segment.h"
#include <fstream>
#include <string>

class BVHReader {
public:
	BVHReader(std::string filename);

    bool loadFile();
    void loadFrame(int frame);
    void applyMotion(std::vector<double> motion);

	void draw();
    int frameSize();
    double getFrameTime();
    Segment *getRoot();

private:
    std::string filename;
    std::ifstream iFile;

    bool isLoaded;
    std::vector<std::unique_ptr<Segment>> root;

    std::vector<std::vector<double>> motion;

    bool loadHierarchy();
    bool loadMotion();
    int channels;
    int frames;
    double frameTime;
};