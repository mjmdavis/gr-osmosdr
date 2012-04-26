/* -*- c++ -*- */
/*
 * Copyright 2012 Dimitri Stolnikov <horiz0n@gmx.net>
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <fstream>
#include <string>
#include <sstream>

#include <boost/assign.hpp>

#include <gr_io_signature.h>

#include <fcd_source.h>

using namespace boost::assign;

fcd_source_sptr make_fcd_source(const std::string &args)
{
    return gnuradio::get_initial_sptr(new fcd_source(args));
}

fcd_source::fcd_source(const std::string &args) :
    gr_hier_block2("fcd_source",
                   gr_make_io_signature (0, 0, 0),
                   gr_make_io_signature (1, 1, sizeof (gr_complex)))
{
    std::string device_name = args;

    if ( device_name.empty() )
    {
        std::vector< std::string > devices = fcd_source::get_devices();

        if ( devices.size() )
            device_name = devices.front(); // pick first available device
        else
            throw std::runtime_error("no FUNcube Dongle device found");
    }

    _src = fcd_make_source_c( device_name );

    connect( _src, 0, self(), 0 );
}

fcd_source::~fcd_source()
{
}

std::vector< std::string > fcd_source::get_devices()
{
    std::vector< std::string > devices;

    std::string line;
    std::ifstream cards( "/proc/asound/cards" );
    if ( cards.is_open() )
    {
        while ( cards.good() )
        {
            getline (cards, line);

            if ( line.find( "USB-Audio - FUNcube Dongle" ) != std::string::npos )
            {
                int id;
                std::istringstream( line ) >> id;

                std::ostringstream hw_id;
                hw_id << "hw:" << id; // build alsa identifier

                devices += hw_id.str();
            }
        }

        cards.close();
    }

    return devices;
}

gr_basic_block_sptr fcd_source::self()
{
    return gr_hier_block2::self();
}

std::string fcd_source::name()
{
    return "FUNcube Dongle";
}

size_t fcd_source::get_num_channels( void )
{
  return 1;
}

osmosdr::meta_range_t fcd_source::get_sample_rates( void )
{
  osmosdr::meta_range_t range;

  range += osmosdr::range_t( get_sample_rate() );

  return range;
}

double fcd_source::set_sample_rate( double rate )
{
  return get_sample_rate();
}

double fcd_source::get_sample_rate( void )
{
  return 96e3;
}

osmosdr::freq_range_t fcd_source::get_freq_range( size_t chan )
{
  osmosdr::freq_range_t range(50e6, 2.2e9);

  return range;
}

double fcd_source::set_center_freq( double freq, size_t chan )
{
  _src->set_freq(float(freq));

  _freq = freq;

  return get_center_freq(chan);
}

double fcd_source::get_center_freq( size_t chan )
{
  return _freq;
}

double fcd_source::set_freq_corr( double ppm, size_t chan )
{
  _src->set_freq_corr( ppm );

  _correct = ppm;

  return get_freq_corr( chan );
}

double fcd_source::get_freq_corr( size_t chan )
{
  return _correct;
}

std::vector<std::string> fcd_source::get_gain_names( size_t chan )
{
  std::vector< std::string > names;

  names += "LNA", "MIXER";

  return names;
}

osmosdr::gain_range_t fcd_source::get_gain_range( size_t chan )
{
  osmosdr::gain_range_t range(-5, 30, 2.5);

  return range;
}

osmosdr::gain_range_t fcd_source::get_gain_range( const std::string & name, size_t chan )
{
  return get_gain_range( chan );
}

double fcd_source::set_gain( double gain, size_t chan )
{
  _src->set_lna_gain(gain);

  _gain = gain;

  return get_gain(chan);
}

double fcd_source::set_gain( double gain, const std::string & name, size_t chan )
{
  return set_gain(chan);
}

double fcd_source::get_gain( size_t chan )
{
  return _gain;
}

double fcd_source::get_gain( const std::string & name, size_t chan )
{
  return get_gain(chan);
}

std::vector< std::string > fcd_source::get_antennas( size_t chan )
{
  std::vector< std::string > antennas;

  antennas += get_antenna(chan);

  return antennas;
}

std::string fcd_source::set_antenna( const std::string & antenna, size_t chan )
{
  return get_antenna(chan);
}

std::string fcd_source::get_antenna( size_t chan )
{
  return "ANT";
}

