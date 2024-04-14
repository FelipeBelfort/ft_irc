/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Libftpp.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 16:38:13 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/11 21:15:02 by TheTerror        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef	LIBFTPP_HPP
# define	LIBFTPP_HPP

# include <iostream>
# include <sstream>
# include <fstream>
# include <fstream>
# include <cstdlib>
# include <string>
# include <cstdio>

class Libftpp
{
	public:

		Libftpp(const Libftpp& other);
		Libftpp&	operator= (const Libftpp& other);
		~Libftpp();

		static std::string		itoa(const size_t& n);
		static std::string&		trimStart(std::string& str, const std::string& set);
		static std::string&		trimStart(std::string& str, const std::string& set, const size_t& nb);
		static std::string&		trimEnd(std::string& str, const std::string& set);
		static std::string&		trimEnd(std::string& str, const std::string& set, const size_t& nb);
		static std::string&		trim(std::string& str, const std::string& set);
		static std::string&		trim(std::string& str, const std::string& set, const size_t& nb);
		static std::string&		removeChars(std::string& str, const std::string& set);
		static bool				strIsWSpaces(const std::string& str);
		static bool				authFileExtension(const std::string& filename, \
									const std::string& extension, std::string& errorfield);
		static std::string		strToLower(std::string str);
		static bool				strIsInt(std::string str);
		static bool				strIsFloat(std::string str);
		static bool				strIsDouble(std::string str);

		static std::string		putFloat(float f);
		static std::string		putDouble(double d);

		/*prints error message on standard error output and returns false*/
		static bool				error(const std::string& msg);
		/*prints perror message on standard error output and returns false*/
		static bool				ft_perror(const std::string& msg);
		/*prints error message on standard error output then throws Texception*/
		template <typename Texception>
		static void				errorThrow(const std::string& msg);
		/*prints error message on standard error output then throws Texception 'e'*/
		template <typename Texception>
		static void				errorThrow(const std::string& msg, const Texception& e);
		/*prints error message on standard error output specifying the file's name
		* then throws Texception*/
		template <typename Texception>
		static void				errorThrow(const std::string& filename, \
										const std::string& msg);
		/*prints error message on standard error output specifying the file's name,
		* the number of line then throws Texception*/
		template <typename Texception>
		static void				errorThrow(const std::string& filename, \
										size_t linenumber, const std::string& msg);
		/*prints error message on standard error output specifying the file's name,
		* the number of line then throws Texception 'e'*/
		template <typename Texception>
		static void				errorThrow(const std::string& filename, \
										size_t linenumber, const Texception& e);

	private:

		Libftpp();
};

/*template function errorThrow()'s implementation*/
#include "Error.tpp"

#endif
/* ************************************************************************** */