/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <q/support/literals.hpp>
#include <q/fx/biquad.hpp>
#include <q_io/audio_file.hpp>
#include <vector>
#include <string>
#include "notes.hpp"

namespace q = cycfi::q;
using namespace q::literals;

void process(std::string name, q::frequency base_freq)
{
   ////////////////////////////////////////////////////////////////////////////
   // Read audio file

   q::wav_reader src{"audio_files/" + name + ".wav"};
   std::uint32_t const sps = src.sps();

   std::vector<float> in(src.length());
   src.read(in);

   constexpr auto n_channels = 4;
   std::vector<float> out(src.length() * n_channels);

   // biquad lowpass filters
   auto lp1 = q::lowpass{ base_freq * 2, sps, 0.51763809 };
   auto lp2 = q::lowpass{ base_freq * 2, sps, 0.70710678 };
   auto lp3 = q::lowpass{ base_freq * 2, sps, 1.9318517 };

   for (auto i = 0; i != in.size(); ++i)
   {
      auto pos = i * n_channels;
      auto ch1 = pos;
      auto ch2 = pos+1;
      auto ch3 = pos+2;
      auto ch4 = pos+3;

      auto s = in[i];

      // Original signal
      out[ch1] = s;

      // Low pass filter 1
      s = lp1(s);
      out[ch2] = s * 3;

      // Low pass filter 2
      s = lp2(s);
      out[ch3] = s * 3;

      // Low pass filter 3
      s = lp3(s);
      out[ch4] = s * 4;
   }

   ////////////////////////////////////////////////////////////////////////////
   // Write to a wav file

   q::wav_writer wav{
      "results/biquad_lp_" + name + ".wav", n_channels, sps
   };
   wav.write(out);
}

int main()
{
   using namespace notes;

   process("1-Low E", low_e);
   process("2-Low E 2th", low_e);
   process("3-A", a);
   process("4-A 12th", a);
   process("5-D", d);
   process("6-D 12th", d);
   process("7-G", g);
   process("8-G 12th", g);
   process("9-B", b);
   process("10-B 12th", b);
   process("11-High E", high_e);
   process("12-High E 12th", high_e);

   process("Tapping D", d);
   process("Hammer-Pull High E", high_e);
   process("Bend-Slide G", g);
   process("GLines1", g);
   process("GLines2", g);
   process("GLines3", g);
   process("GStaccato", g);

   return 0;
}