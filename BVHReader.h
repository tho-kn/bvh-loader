#include "Segment.h"
#include <fstream>
#include <string>

using namespace std;
typedef vector<vector<double>> Motion;

class BVHReader {
public:
	BVHReader(string filename);

    bool loadFile();
    vector<unique_ptr<Segment>>* getRoots();
    Motion getMotion();

    int getChannels();
    int frameSize();
    double getFrameTime();

    bool loaded();

private:
    string filename;
    ifstream iFile;

    bool isLoaded;
    vector<unique_ptr<Segment>> root;
    Motion motion;

    bool loadHierarchy();
    bool loadMotion();
    int channels;
    int frames;
    double frameTime;
};