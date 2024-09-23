//
// Created by User on 20/09/2024.
//

#ifndef UEDOXCONFIG_H
#define UEDOXCONFIG_H

#ifndef UEDOXYGEN_ASSERT
#include <iostream>
#define UEDOXYGEN_ASSERT(_EXPR, _MSG)                                                                                  \
  if (!(_EXPR))                                                                                                        \
  {                                                                                                                    \
    std::cerr << _MSG << std::endl;                                                                                    \
  }
#endif

/*
 *#ifndef UEDOXYGEN_INFO
 *#define UEDOXYGEN_INFO(_MSG) std::cout << _MSG << std::endl;
 *#endif
 */

#endif
