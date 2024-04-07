/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:33:15 by TheTerror         #+#    #+#             */
/*   Updated: 2024/02/26 16:39:04 by TheTerror        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

template<typename Texception>
void			Libftpp::errorThrow(const std::string& msg)
{
	try
	{
		throw(Texception());
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << msg << ", " << e.what() << '\n';
		throw;
	}
}

template<typename Texception>
void			Libftpp::errorThrow(const std::string& msg, const Texception& e)
{
	std::cerr << "Error: " << msg << ", " << e.what() << '\n';
	throw;
}

template<typename Texception>
void			Libftpp::errorThrow(const std::string& filename, \
		const std::string& msg)
{
	try
	{
		throw(Texception());
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: \"" << filename << "\": " << msg << ", " \
				<< e.what() << '\n';
		throw;
	}
}

template<typename Texception>
void			Libftpp::errorThrow(const std::string& filename, \
		size_t linenumber, const std::string& msg)
{
	try
	{
		throw(Texception());
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: \"" << filename << "\": at line " \
				<< Libftpp::itoa(linenumber) << ": " << msg << ", " \
				<< e.what() << '\n';
		throw;
	}
}

template<typename Texception>
void			Libftpp::errorThrow(const std::string& filename, \
		size_t linenumber, const Texception& e)
{
	std::cerr << "Error: \"" << filename << "\": at line " \
			<< Libftpp::itoa(linenumber) << ": " << e.what() << '\n';
	throw;
}
