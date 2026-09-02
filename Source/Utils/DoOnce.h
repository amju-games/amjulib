// ** AMJULIB **
// (c) Copyright 2000-2026 Juliet Colman

#pragma once

namespace Amju
{
// ** do_once **
// Easy-to-use wrapper around a block of code so the block is only
//  executed the first time it is encountered.
// 
// Example:
//   do_once
//   {
//     do_something_that_only_works_once();
//   }
//
// BUT watch out! This doesn't mean "once for each instance", it
//  means "once for the process".

#define do_once static int the_once_ler = 0; if (the_once_ler++ == 0)
}
