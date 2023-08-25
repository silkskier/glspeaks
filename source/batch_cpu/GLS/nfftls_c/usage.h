/** \file usage.h
 * User interface.
 *
 * \author B. Leroy
 *
 * This file is part of nfftls.
 *
 * Nfftls is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nfftls is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with nfftls.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2012 by B. Leroy
 */
#ifndef USAGE_H
#define USAGE_H

/** Control parameter values. */
typedef struct
{
  const char* datafile;		//!< Input data file.
  const char* outfile;		//!< Ouput periodogram file.
  double oversampling;		//!< Oversampling factor.
  double hifactor;		//!< High frequency control factor.
  int nfreqs;			//!< Number of (independent) frequencies.
  const char* cmdline;		//!< Command line.
  const char* version;		//!< Version string.
} Options;

/**
 * Gets the options from the command line and
 * intialises the corresponding variables.
 *
 * \param[in] argc the number of arguments on the command line.
 * \param[in] argv the list of arguments on the command line.
 * \param[out] cmdline the command line.
 * \param[out] opts the object that will hold the option values
 *             (possibly default values).
 */
int getOptions(int argc, char** argv, Options* opts);

/**
 * Sets the default values for the oversampling and
 * the high frequency control factor.
 *
 * \param[out] opts the object storing the command line option values.
 * \param[in] over the oversampling.
 * \param[in] hifac the high frequency control parameter.
 */
void initOptions(Options* opts, double over, double hifac);

/**
 * Frees the memory allocated during
 * command line handling.
 *
 * \param p the object storing the command line option values.
 */
void cleanupOptions(Options* opts);

#endif /* !USAGE_H */
