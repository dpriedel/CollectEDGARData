// =====================================================================================
//
//       Filename:  HTTPS_Connection.h
//
//    Description:  Class provides read-only acces to an HTTPS server.
//
//        Version:  1.0
//        Created:  01/14/2014 10:08:13 AM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  David P. Riedel (dpr), driedel@cox.net
//        License:  GNU General Public License v3
//        Company:
//
// =====================================================================================

	/* This file is part of CollectEDGARData. */

	/* CollectEDGARData is free software: you can redistribute it and/or modify */
	/* it under the terms of the GNU General Public License as published by */
	/* the Free Software Foundation, either version 3 of the License, or */
	/* (at your option) any later version. */

	/* CollectEDGARData is distributed in the hope that it will be useful, */
	/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
	/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
	/* GNU General Public License for more details. */

	/* You should have received a copy of the GNU General Public License */
	/* along with CollectEDGARData.  If not, see <http://www.gnu.org/licenses/>. */


#ifndef HTTPS_CONNECTION_H_
#define HTTPS_CONNECTION_H_

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

#include <Poco/URI.h>
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/SharedPtr.h"

class SSLInitializer
{
public:
	SSLInitializer()
	{
		Poco::Net::initializeSSL();
	}

	~SSLInitializer()
	{
		Poco::Net::uninitializeSSL();
	}
};


// =====================================================================================
//        Class:  HTTPS_Server
//  Description:  provides read-only access to an HTTPS server
// =====================================================================================
class HTTPS_Server
{
	public:
		// ====================  LIFECYCLE     =======================================
		HTTPS_Server ()=delete;                             // constructor
		HTTPS_Server(const std::string& server_name);
		~HTTPS_Server(void);

		// ====================  ACCESSORS     =======================================

		bool HaveActiveSession(void) const { return session_ && session_->connected(); }
		const std::string& GetWorkingDirectory(void) const { return path_;}

		// ====================  MUTATORS      =======================================

		void OpenHTTPSConnection(void);
		void CloseHTTPSConnection(void);
		void ChangeWorkingDirectoryTo(const std::string& directory_name);
		void DownloadFile(const std::string& remote_file_name, const fs::path& local_file_name);
		void DownloadBinaryFile(const std::string& remote_file_name, const fs::path& local_file_name);

		std::vector<std::string> ListWorkingDirectoryContents(void);

		// ====================  OPERATORS     =======================================

	protected:
		// ====================  DATA MEMBERS  =======================================

        std::string InteractWithServer(const std::string& request);

	private:
		// ====================  DATA MEMBERS  =======================================

		Poco::URI server_uri_;
		std::string server_name_;
		std::string path_;

		// Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrCert_; // ask the user via console
		Poco::SharedPtr<Poco::Net::AcceptCertificateHandler> ptrCert_; // ask the user via console
		Poco::Net::Context::Ptr ptrContext_;

		SSLInitializer* ssl_initializer_;
		Poco::Net::HTTPSClientSession* session_;
}; // -----  end of class HTTPS_Server  -----

#endif /* HTTPS_CONNECTION_H_ */
