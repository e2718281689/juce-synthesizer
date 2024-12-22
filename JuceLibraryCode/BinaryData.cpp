/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== README.md ==================
static const unsigned char temp_binary_data_0[] =
"# PolyBLEP Oscillator\n"
"C++ port of the [PolyBLEP oscillator by Tale](http://www.taletn.com/reaper/mono_synth/).\n"
"\n"
"Includes a wrapper (`PolyBLEPStk`) to use the oscillator with components from [The Synthesis Toolkit](https://ccrma.stanford.edu/software/stk/index.html). To use it with [JUCE](http://juce.com), install the [stk_module](https://github.com/drowaudio/s"
"tk_module).\n"
"\n"
"## License\n"
"```\n"
"This software is provided 'as-is', without any express or implied\n"
"warranty.  In no event will the authors be held liable for any damages\n"
"arising from the use of this software.\n"
"\n"
"Permission is granted to anyone to use this software for any purpose,\n"
"including commercial applications, and to alter it and redistribute it\n"
"freely, subject to the following restrictions:\n"
"\n"
"1. The origin of this software must not be misrepresented; you must not\n"
"   claim that you wrote the original software. If you use this software\n"
"   in a product, an acknowledgment in the product documentation would be\n"
"   appreciated but is not required.\n"
"2. Altered source versions must be plainly marked as such, and must not be\n"
"   misrepresented as being the original software.\n"
"3. This notice may not be removed or altered from any source distribution.\n"
"```\n";

const char* README_md = (const char*) temp_binary_data_0;

//================== developer_notes.md ==================
static const unsigned char temp_binary_data_1[] =
"### Developer Notes\n"
"\n"
"#### JavaScript export\n"
"\n"
"Maximilian exports functions using two (!) different systems: Emscripten and Cheerp.  Emscripten works well for more complex/heavyweight processes, but the overhead for function calls means that it's inefficient for small dsp functions, and there are"
" also some issues with memory management. Instead, we use Cheerp, which translates these functions directly into JavaScript, and keeps everything in JavaScript memory space.\n"
"\n"
"##### Exporting using Emscripten\n"
"\n"
"[TBC]\n"
"\n"
"##### Exporting using Cheerp\n"
"\n"
"1. add CHEERP_EXPORT to your clasd definition\n"
"\n"
"```\n"
"class CHEERP_EXPORT myClass {\n"
"\n"
"}\n"
"```\n"
"\n"
"2. Make sure that the constructor is included in the .cpp file (this is a quirk or Cheerp) - see the end of maximilian.cpp for examples.\n"
"\n"
"3. Cheerp does not work well with overridden functions.\n"
"\n"
"\n"
"#### Building Documentation\n"
"\n"
"We use doxygen to build xml documentation from comments in the C++ source code, and then use sphinx and breathe to build this into a website\n"
"\n"
"The files are in the ``docs/docbuild`` folder.\n"
"\n"
"You will need to install doxygen, and then\n"
"\n"
"```\n"
"pip install sphinx\n"
"pip install sphinx_rtd_theme\n"
"pip install breathe\n"
"```\n"
"\n"
"\n"
"To build the documentation:\n"
"\n"
"```\n"
"doxygen doxygen.config\n"
"make html\n"
"mv -u _build/html/* ..\n"
"```";

const char* developer_notes_md = (const char*) temp_binary_data_1;

//================== LICENSE.txt ==================
static const unsigned char temp_binary_data_2[] =
"Copyright 2009 Mick Grierson\n"
"\n"
"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, mo"
"dify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
"\n"
"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
"\n"
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOL"
"DERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n";

const char* LICENSE_txt = (const char*) temp_binary_data_2;

//================== README.md ==================
static const unsigned char temp_binary_data_3[] =
"```\n"
"  _____ _____ ___   __ _  _____  __ __   __ ____  ____\n"
" /     \\\\_   \\\\  \\/  /  |/     \\|  |  | |  \\_   \\/    \\\n"
"|  Y Y  \\/ /_ \\>    <|  |  Y Y  \\  |  |_|  |/ /_ \\  Y  \\\n"
"|__|_|  (___  /__/\\__\\__|__|_|  /__|____/__(___  /__|  /\n"
"      \\/    \\/                \\/               \\/    \\/\n"
"```\n"
"![version](https://img.shields.io/badge/version-2.4-red)\n"
"[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/mimic-sussex/eppEditor/blob/master/LICENSE)\n"
"\n"
"<br />\n"
"\n"
"### What's Maximilian?\n"
"\n"
"Maximilian is a cross-platform and multi-target audio synthesis and signal processing library. It was written in C++ and provides bindings to Javascript. It's compatible with native implementations for MacOS, Windows, Linux and iOS systems, as well a"
"s client-side browser-based applications. Maximilian is self-contained, and compiles without dependencies. The main features are:\n"
"\n"
"- sample playback, recording and looping\n"
"- support for WAV and OGG files.\n"
"- a selection of oscillators and filters\n"
"- enveloping\n"
"- multichannel mixing for 1, 2, 4 and 8 channel setups\n"
"- controller mapping functions\n"
"- effects including delay, distortion, chorus, flanging\n"
"- granular synthesis, including time and pitch stretching\n"
"- atom synthesis\n"
"- real-time music information retrieval functions: spectrum analysis, spectral features, octave analysis, Bark scale analysis, and MFCCs\n"
"- example projects for Windows and MacOS, susing command line and OpenFrameworks environments\n"
"- example projects for Firefox and Chromium-based browsers using the Web Audio API ScriptProcessorNode (deprecated!)\n"
"- example projects for Chromium-based browsers using the Web Audio API AudioWorklet (e.g. Chrome, Brave, Edge, Opera, Vivaldi)\n"
"- will run on embedded systems (e.g. ESP32, Pi Pico)\n"
"\n"
"### Documentation\n"
"\n"
"in [docs/index.html](docs/index.html)\n"
"\n"
"\n"
"### Basic Examples\n"
"\n"
"\n"
"\n"
"Examples demonstrating different features can be found in the maximilian_examples folder.  Each example is in a subfolder, and can be built as follows using cmake:\n"
"\n"
"```\n"
"cd [example folder]\n"
"mkdir build\n"
"cd build\n"
"cmake ..\n"
"make\n"
"./maximilian\n"
"```\n"
"\n"
"### Web Audio\n"
"\n"
"A transpiled javascript version of the library is included in this repository, for both Script Processor Nodes and AudioWorklets. Try this out at (https://mimicproject.com/guides/maximJS).\n"
"\n"
"To run this on your on site, locally, or on GitHub Pages, check out [this repo](https://github.com/Louismac/maximilian-js-local).\n"
"\n"
"\n"
"### Mac OS XCode Project\n"
"\n"
"You can run the examples using the 'maximilianTest' XCode 3 project provided.\n"
"\n"
"\n"
"### MS Windows Visual Studio Project\n"
"\n"
"This is in the maximilianTestWindowsVS2010 folder. You will need to install the DirectX SDK, so that the program can use DirectSound.\n"
"\n"
"\n"
"### Command Line Compilation in Mac OS\n"
"\n"
"> g++ -Wall -D__MACOSX_CORE__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -framework CoreAudio -framework CoreFoundation -lpthread\n"
"\n"
"For M1 Mac OS\n"
"> g++ -Wall -D__MACOSX_CORE__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -framework CoreAudio -framework CoreServices -framework AudioToolbox -framework AudioUnit -framework Accelerate -lpthread\n"
"\n"
"> ./maximilian\n"
"\n"
"\n"
"### Command Line Compilation in Linux\n"
"\n"
"With OSS:\n"
"> g++ -Wall -D__LINUX_OSS__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -lpthread\n"
"\n"
"With ALSA:\n"
"> g++ -Wall -D__LINUX_ALSA__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -lasound -lpthread\n"
"\n"
"With Jack:\n"
"> g++ -Wall -D__UNIX_JACK__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp `pkg-config --cflags --libs jack` -lpthread\n"
"\n"
"then:\n"
"> ./maximilian\n"
"\n"
"\n"
"\n"
"### OpenFrameworks Project\n"
"\n"
"Maximilian works well with the OpenFrameworks C++ creative coding toolkit (http://www.openframeworks.cc).\n"
"\n"
"In the ofxMaxim directory you will find examples to run in Windows, OSX and iOS, including FFT analysis and granular synthesis.\n"
"\n"
"You can install the ofxMaxim addon by copying the ofxMaxim/ofxMaxim folder into your openframeworks addons directory.\n"
"\n"
"Important: when using Maximilian on OSX, link against the Accelerate framework.\n"
"\n"
"\n"
"\n"
"### Developer Notes\n"
"\n"
"[see developer_notes.md](developer_notes.md)\n"
"\n"
"\n"
"\n"
"\n"
"\n";

const char* README_md2 = (const char*) temp_binary_data_3;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x64791dc8:  numBytes = 1213; return README_md;
        case 0x05878caa:  numBytes = 1259; return developer_notes_md;
        case 0x5a320952:  numBytes = 1053; return LICENSE_txt;
        case 0x2aaa9b6a:  numBytes = 4118; return README_md2;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "README_md",
    "developer_notes_md",
    "LICENSE_txt",
    "README_md2"
};

const char* originalFilenames[] =
{
    "README.md",
    "developer_notes.md",
    "LICENSE.txt",
    "README.md"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
