/* 

                          Firewall Builder

                 Copyright (C) 2001 NetCitadel, LLC

  Author:  Vadim Kurland     vadim@fwbuilder.org

  $Id$


  This program is free software which we release under the GNU General Public
  License. You may redistribute and/or modify this program under the terms
  of that license as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  To get a copy of the GNU General Public License, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/




#ifndef __CUSTOMSERVICE_HH_FLAG__
#define __CUSTOMSERVICE_HH_FLAG__

#include <fwbuilder/Service.h>

#include <map>

namespace libfwbuilder
{

class CustomService : public Service
{
    private:
    
    std::map<std::string,std::string>   codes;
    
    public:
    
    CustomService();
    CustomService(const FWObjectDatabase *root,bool prepopulate);
    virtual ~CustomService();
    
    virtual void       fromXML(xmlNodePtr parent) throw(FWException);
    virtual xmlNodePtr toXML(xmlNodePtr parent) throw(FWException);
    virtual bool cmp(const FWObject *obj) throw(FWException);
    
    virtual FWObject& shallowDuplicate(const FWObject *obj, bool preserve_id = true) throw(FWException);
    
    DECLARE_FWOBJECT_SUBTYPE(CustomService);

    virtual std::string getProtocolName();
    virtual int    getProtocolNumber();

    void setCodeForPlatform(const std::string& platform, const std::string& code);
    const std::string& getCodeForPlatform(const std::string& platform);
};

}

#endif




