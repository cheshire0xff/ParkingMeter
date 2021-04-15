/*
 * Config.h
 *
 *  Created on: 15 Apr 2021
 *      Author: cheshire
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

namespace Config
{
#ifdef NDEBUG
constexpr auto logsEnabled = false;
#else
constexpr auto logsEnabled = true;
#endif

}


#endif /* INC_CONFIG_H_ */
