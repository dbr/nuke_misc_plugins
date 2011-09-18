#include "DDImage/DeepReader.h"
#include "DDImage/DeepPlane.h"

#include "DDImage/Knobs.h"
#include "DDImage/Thread.h"

#include <DDImage/ChannelSet.h>


using namespace DD::Image;

namespace nuke_misc_plugins {

class ratDeepReaderFormat : public DeepReaderFormat
{

    friend class ratDeepReader;

    bool _raw;
    bool _premult;

public:

    ratDeepReaderFormat()
    {
        _raw = false;
        _premult = false;
    }

    bool getRaw() const
    {
        return _raw;
    }

    bool getPremult() const
    {
        return _premult;
    }

    void knobs(Knob_Callback f)
    {
        Bool_knob(f, &_raw, "raw", "raw values");

        Bool_knob(f, &_premult, "premult", "premultiply");
        Tooltip(f, "Premultiply values from file (if off, then it assumes they are already premultiplied)");
    }

    void append(Hash& hash)
    {
        hash.append(_raw);
        hash.append(_premult);
    }
};


/**
* This is a plugin to read the Pixar Renderman 'dtex' files.
*/
class ratDeepReader : public DeepReader
{
    Lock _engineLock;
    int _numChans;
    DD::Image::OutputContext _outputContext;

public:
    ratDeepReader(DeepReaderOwner* op, const std::string& filename) : DeepReader(op)
    {
        _outputContext = _owner->readerOutputContext();
        setInfo(/*width*/ 10, /*height*/ 10, _outputContext, Mask_RGB | Mask_Alpha | Mask_Deep);
    }

    ~ratDeepReader()
    {
    }

    void doDeepEngine(DD::Image::Box box, const ChannelSet& channels, DeepOutputPlane& outPlane)
    {
        ratDeepReaderFormat* format = dynamic_cast<ratDeepReaderFormat*>(_owner->handler());
        //bool raw = format ? format->getRaw() : false;
        outPlane = DeepOutputPlane(channels, box);

        for (Box::iterator it = box.begin(); it != box.end(); it++)
        {
            float x = it.x;
            float y = it.y;
            _outputContext.from_proxy_xy(x, y);
            DD::Image::DeepOutPixel pels(4 * channels.size());
            foreach(chan, channels){
                pels.push_back(1);
                pels.push_back(2);
                pels.push_back(3);
                pels.push_back(4);
            }

            outPlane.addPixel(pels);
        }
    }
};

static DeepReader* build(DeepReaderOwner* op, const std::string& fn)
{
    return new ratDeepReader(op, fn);
}

static DeepReaderFormat* buildFormat(DeepReaderOwner* op)
{
    return new ratDeepReaderFormat();
}

static const DeepReader::Description d("rat\0", "rat", build, buildFormat);

}
