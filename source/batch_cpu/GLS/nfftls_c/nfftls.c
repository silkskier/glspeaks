/** \file nfftls.c
 * Computation of an NFFT-based periodogram.
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
 *
 * \if _svnid
 * $Id: nfftls.c 84 2012-07-24 08:23:28Z leroy $
 * \endif
 */
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <errno.h>
#include <string.h>

#include "io.h"
#include "nfft.h"
#include "utils.h"
#include "ls.h"
#include "usage.h"

int main(int argc, char** argv)
{
  Options opts;

  /* Set default values: oversampling = 4.0, hifactor = 1.0. */
  initOptions(&opts, 4.0, 1.0);
  
  int ind = getOptions(argc, argv, &opts);
  if (ind < argc)
  {
    Message(MSG_WARNING, "The following extra arguments will be ignored:");
    do
    {
      fprintf(stderr, "    %s\n", argv[ind]);
    } while (++ind < argc);
  }

  double* x = NULL;
  double* y = NULL;

 /* Default was 1000 data entries, sets it to 100000. */
  setReadDataFileIncrement(100000);

  int npts = readDataFile(opts.datafile, &x, &y);
  if (npts <= 0)
    Message(MSG_ERROR, "Error reading file %s", opts.datafile);

  /*
   * Computes the periodogram.
   * The timing of the algorithm is done
   * within function periodogram().
   */
  LS* ls = periodogram(x, y, npts, opts.oversampling, opts.hifactor);

  /* Outputs the results either to stdout or to a file. */
  FILE* fp = stdout;
  if (opts.outfile)
  {
    fp = fopen(opts.outfile, "w");
    if (!fp)
      Message(MSG_ERROR, "Can't open file %s", opts.outfile);
  }
  
  fprintf(fp,
	  "%s\n#---\n"
	  "# Oversampling: %g\n"
	  "# Hifactor: %g\n#---\n"
	  "#%14s%15s\n",
	  opts.cmdline, opts.oversampling, opts.hifactor,
	  "f", "nfft");

  for (int k = 0; k < ls->nfreqs; k++)
    fprintf(fp, "%15.6f%15.6e\n", ls->freqs[k], ls->Pn[k]);

  fprintf(fp,
	  "# Maximum peak at f = %.6f\n"
	  "# Probability of peak due to white noise: %.6e\n",
	  ls->freqs[ls->jmax], ls->proba);

#ifdef BENCHMARK
  fprintf(fp,
	  "# Time used for computing periodogram: %g s, "
	  "npts = %d, over = %g\n",
	  ls->tused,
	  npts, opts.oversampling);
#endif

  if (fp != stdout)
    fclose(fp);

  LSfree(ls);
  free(y);
  free(x);
  cleanupOptions(&opts);
  
  return EXIT_SUCCESS;
}
