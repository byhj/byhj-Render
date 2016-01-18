#ifndef Mgr_H
#define Mgr_H

#include <string>

namespace byhj
{

  class Mgr
  {
  public:
	  Mgr();
	  virtual ~Mgr();

	  void setDir(std::string dir);
	  std::string getDir() const;

  protected:
	  std::string m_dir = "../../media/";
  };

}
#endif