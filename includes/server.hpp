#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include "location.hpp"

class Server;

using t_locmap = std::map<std::string, Location>;
using t_servmap =  std::vector<Server>;
using t_vecstr = std::vector<std::string>;

//TODO IP should probably always be 127.0.0.1 then at least one port should be set per server. A server might also have servernames.

class Server
{
public:
	// Constructors
	Server();
//	Server(const Server &copy);

	// Destructor
	~Server();

	// Operators
//	Server & operator=(const Server &assign);

	// Getters
//	std::vector<int>	getServerSocket() const;
//	const std::string&			getServerIp() const;
	int					getClientBodySize() const;
	const std::vector<int>&	getServerListen() const;
	const t_vecstr&			getErrorPage() const;
	const t_vecstr&			getServerNames() const;
//	t_locmap			getLocationMap() const;
//	std::string			getServerRoot() const;
//	Location			*getLocation(int port, std::string url) const;

	// Setters
	void	setServerIp(const Json& json);
	void	addServerListen(const Json& json);
	void	addServerName(const Json& json);
	void	setServerClientBodySize(const Json& json);
	void	setServerErrorPage(const Json& json);
//	void	setServerRoot(std::string root);
//	void	setServerSocket(int server_socket);

	// Member Functions
	void	addLocationToServer(std::string location_dir);

	typedef void (Server::* Func )(const Json&);
	//Table to check which function to jump in
	typedef struct s_table {
		std::string	key;
		Json::Token	type;
		Func		map_values;
	}	t_table;

	Func set_values(const std::string name, const Json& json);

	Location	*loc = NULL;
public:
	t_locmap			_location;
private:
	int					_client_body_size;
	std::vector<int>	_server_fd;
	std::vector<int>	_server_listen;
	t_vecstr			_error_page;
	t_vecstr			_server_name;
//	std::string			_root;
//	std::string			_server_ip;
};

#endif