/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== README.md ==================
static const unsigned char temp_binary_data_0[] =
"# PolyBLEP Oscillator\r\n"
"C++ port of the [PolyBLEP oscillator by Tale](http://www.taletn.com/reaper/mono_synth/).\r\n"
"\r\n"
"Includes a wrapper (`PolyBLEPStk`) to use the oscillator with components from [The Synthesis Toolkit](https://ccrma.stanford.edu/software/stk/index.html). To use it with [JUCE](http://juce.com), install the [stk_module](https://github.com/drowaudio/s"
"tk_module).\r\n"
"\r\n"
"## License\r\n"
"```\r\n"
"This software is provided 'as-is', without any express or implied\r\n"
"warranty.  In no event will the authors be held liable for any damages\r\n"
"arising from the use of this software.\r\n"
"\r\n"
"Permission is granted to anyone to use this software for any purpose,\r\n"
"including commercial applications, and to alter it and redistribute it\r\n"
"freely, subject to the following restrictions:\r\n"
"\r\n"
"1. The origin of this software must not be misrepresented; you must not\r\n"
"   claim that you wrote the original software. If you use this software\r\n"
"   in a product, an acknowledgment in the product documentation would be\r\n"
"   appreciated but is not required.\r\n"
"2. Altered source versions must be plainly marked as such, and must not be\r\n"
"   misrepresented as being the original software.\r\n"
"3. This notice may not be removed or altered from any source distribution.\r\n"
"```\r\n";

const char* README_md = (const char*) temp_binary_data_0;

//================== developer_notes.md ==================
static const unsigned char temp_binary_data_1[] =
"### Developer Notes\r\n"
"\r\n"
"#### JavaScript export\r\n"
"\r\n"
"Maximilian exports functions using two (!) different systems: Emscripten and Cheerp.  Emscripten works well for more complex/heavyweight processes, but the overhead for function calls means that it's inefficient for small dsp functions, and there are"
" also some issues with memory management. Instead, we use Cheerp, which translates these functions directly into JavaScript, and keeps everything in JavaScript memory space.\r\n"
"\r\n"
"##### Exporting using Emscripten\r\n"
"\r\n"
"[TBC]\r\n"
"\r\n"
"##### Exporting using Cheerp\r\n"
"\r\n"
"1. add CHEERP_EXPORT to your clasd definition\r\n"
"\r\n"
"```\r\n"
"class CHEERP_EXPORT myClass {\r\n"
"\r\n"
"}\r\n"
"```\r\n"
"\r\n"
"2. Make sure that the constructor is included in the .cpp file (this is a quirk or Cheerp) - see the end of maximilian.cpp for examples.\r\n"
"\r\n"
"3. Cheerp does not work well with overridden functions.\r\n"
"\r\n"
"\r\n"
"#### Building Documentation\r\n"
"\r\n"
"We use doxygen to build xml documentation from comments in the C++ source code, and then use sphinx and breathe to build this into a website\r\n"
"\r\n"
"The files are in the ``docs/docbuild`` folder.\r\n"
"\r\n"
"You will need to install doxygen, and then\r\n"
"\r\n"
"```\r\n"
"pip install sphinx\r\n"
"pip install sphinx_rtd_theme\r\n"
"pip install breathe\r\n"
"```\r\n"
"\r\n"
"\r\n"
"To build the documentation:\r\n"
"\r\n"
"```\r\n"
"doxygen doxygen.config\r\n"
"make html\r\n"
"mv -u _build/html/* ..\r\n"
"```";

const char* developer_notes_md = (const char*) temp_binary_data_1;

//================== LICENSE.txt ==================
static const unsigned char temp_binary_data_2[] =
"Copyright 2009 Mick Grierson\r\n"
"\r\n"
"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, mo"
"dify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\r\n"
"\r\n"
"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\r\n"
"\r\n"
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOL"
"DERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\r\n";

const char* LICENSE_txt = (const char*) temp_binary_data_2;

//================== README.md ==================
static const unsigned char temp_binary_data_3[] =
"```\r\n"
"  _____ _____ ___   __ _  _____  __ __   __ ____  ____\r\n"
" /     \\\\_   \\\\  \\/  /  |/     \\|  |  | |  \\_   \\/    \\\r\n"
"|  Y Y  \\/ /_ \\>    <|  |  Y Y  \\  |  |_|  |/ /_ \\  Y  \\\r\n"
"|__|_|  (___  /__/\\__\\__|__|_|  /__|____/__(___  /__|  /\r\n"
"      \\/    \\/                \\/               \\/    \\/\r\n"
"```\r\n"
"![version](https://img.shields.io/badge/version-2.4-red)\r\n"
"[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/mimic-sussex/eppEditor/blob/master/LICENSE)\r\n"
"\r\n"
"<br />\r\n"
"\r\n"
"### What's Maximilian?\r\n"
"\r\n"
"Maximilian is a cross-platform and multi-target audio synthesis and signal processing library. It was written in C++ and provides bindings to Javascript. It's compatible with native implementations for MacOS, Windows, Linux and iOS systems, as well a"
"s client-side browser-based applications. Maximilian is self-contained, and compiles without dependencies. The main features are:\r\n"
"\r\n"
"- sample playback, recording and looping\r\n"
"- support for WAV and OGG files.\r\n"
"- a selection of oscillators and filters\r\n"
"- enveloping\r\n"
"- multichannel mixing for 1, 2, 4 and 8 channel setups\r\n"
"- controller mapping functions\r\n"
"- effects including delay, distortion, chorus, flanging\r\n"
"- granular synthesis, including time and pitch stretching\r\n"
"- atom synthesis\r\n"
"- real-time music information retrieval functions: spectrum analysis, spectral features, octave analysis, Bark scale analysis, and MFCCs\r\n"
"- example projects for Windows and MacOS, susing command line and OpenFrameworks environments\r\n"
"- example projects for Firefox and Chromium-based browsers using the Web Audio API ScriptProcessorNode (deprecated!)\r\n"
"- example projects for Chromium-based browsers using the Web Audio API AudioWorklet (e.g. Chrome, Brave, Edge, Opera, Vivaldi)\r\n"
"- will run on embedded systems (e.g. ESP32, Pi Pico)\r\n"
"\r\n"
"### Documentation\r\n"
"\r\n"
"in [docs/index.html](docs/index.html)\r\n"
"\r\n"
"\r\n"
"### Basic Examples\r\n"
"\r\n"
"\r\n"
"\r\n"
"Examples demonstrating different features can be found in the maximilian_examples folder.  Each example is in a subfolder, and can be built as follows using cmake:\r\n"
"\r\n"
"```\r\n"
"cd [example folder]\r\n"
"mkdir build\r\n"
"cd build\r\n"
"cmake ..\r\n"
"make\r\n"
"./maximilian\r\n"
"```\r\n"
"\r\n"
"### Web Audio\r\n"
"\r\n"
"A transpiled javascript version of the library is included in this repository, for both Script Processor Nodes and AudioWorklets. Try this out at (https://mimicproject.com/guides/maximJS).\r\n"
"\r\n"
"To run this on your on site, locally, or on GitHub Pages, check out [this repo](https://github.com/Louismac/maximilian-js-local).\r\n"
"\r\n"
"\r\n"
"### Mac OS XCode Project\r\n"
"\r\n"
"You can run the examples using the 'maximilianTest' XCode 3 project provided.\r\n"
"\r\n"
"\r\n"
"### MS Windows Visual Studio Project\r\n"
"\r\n"
"This is in the maximilianTestWindowsVS2010 folder. You will need to install the DirectX SDK, so that the program can use DirectSound.\r\n"
"\r\n"
"\r\n"
"### Command Line Compilation in Mac OS\r\n"
"\r\n"
"> g++ -Wall -D__MACOSX_CORE__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -framework CoreAudio -framework CoreFoundation -lpthread\r\n"
"\r\n"
"For M1 Mac OS\r\n"
"> g++ -Wall -D__MACOSX_CORE__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -framework CoreAudio -framework CoreServices -framework AudioToolbox -framework AudioUnit -framework Accelerate -lpthread\r\n"
"\r\n"
"> ./maximilian\r\n"
"\r\n"
"\r\n"
"### Command Line Compilation in Linux\r\n"
"\r\n"
"With OSS:\r\n"
"> g++ -Wall -D__LINUX_OSS__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -lpthread\r\n"
"\r\n"
"With ALSA:\r\n"
"> g++ -Wall -D__LINUX_ALSA__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -lasound -lpthread\r\n"
"\r\n"
"With Jack:\r\n"
"> g++ -Wall -D__UNIX_JACK__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp `pkg-config --cflags --libs jack` -lpthread\r\n"
"\r\n"
"then:\r\n"
"> ./maximilian\r\n"
"\r\n"
"\r\n"
"\r\n"
"### OpenFrameworks Project\r\n"
"\r\n"
"Maximilian works well with the OpenFrameworks C++ creative coding toolkit (http://www.openframeworks.cc).\r\n"
"\r\n"
"In the ofxMaxim directory you will find examples to run in Windows, OSX and iOS, including FFT analysis and granular synthesis.\r\n"
"\r\n"
"You can install the ofxMaxim addon by copying the ofxMaxim/ofxMaxim folder into your openframeworks addons directory.\r\n"
"\r\n"
"Important: when using Maximilian on OSX, link against the Accelerate framework.\r\n"
"\r\n"
"\r\n"
"\r\n"
"### Developer Notes\r\n"
"\r\n"
"[see developer_notes.md](developer_notes.md)\r\n"
"\r\n"
"\r\n"
"\r\n"
"\r\n"
"\r\n";

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
        case 0x64791dc8:  numBytes = 1236; return README_md;
        case 0x05878caa:  numBytes = 1305; return developer_notes_md;
        case 0x5a320952:  numBytes = 1060; return LICENSE_txt;
        case 0x2aaa9b6a:  numBytes = 4232; return README_md2;
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
