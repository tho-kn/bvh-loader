#include "BVHViewer.h"
#include <stack>

using namespace std;

BVHViewer::BVHViewer(std::vector<std::unique_ptr<Segment>> roots, Motion motion, int channels):
    root(move(roots)), motion(motion), channels(channels){}

void BVHViewer::loadFrame(int frame){
    if(frame >= this->frameSize()){
        frame %= this->frameSize();
    }

    vector<double> data = this->motion[frame];
    int dataPtr = 0;

    stack<Segment *> segQ;
    for(auto iter = this->root.begin(); iter < this->root.end(); iter++){
        segQ.push((*iter).get());
    }

    while(segQ.size() > 0){
        Segment *curr = segQ.top();
        segQ.pop();

        int channels = curr->numChannels();
        for(int i = 0; i < channels; i++){
            curr->applyChannel(data[dataPtr], i);
            dataPtr++;
        }

        int segs = curr->numSub();
        for(int i = segs - 1; i >= 0; i--){
            segQ.push(curr->getSeg(i));
        }
    }
}

void BVHViewer::draw(){
    auto iter = this->root.begin();
    for(; iter != this->root.end(); ++iter){
        (*iter)->draw();
    }
}

int BVHViewer::frameSize(){
    return this->motion.size();
}
