//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __PARAMETER_HPP__
#define __PARAMETER_HPP__


#include <iostream>
#include <fstream>

using namespace std;

class DoubleParameter{
      public:
        DoubleParameter(double val, double min, double max, double step, std::string name)  {
          _value = val;
          _min = min;
          _max = max;
          _step = step;
          _name = name;
        };
        void        set(double val)        {_value = val;};
        double      get()             {return _value;};
        double      min()             {return _min;};
        double      max()             {return _max;};
        double      step()            {return _step;};
        std::string name()            {return _name;};
        void        incr()            {if (_value+_step <= _max) _value = _value + _step; else _value = _max; };
        void        decr()            {if (_value-_step >= _min) _value = _value - _step; else _value = _min; };
        
        std::string getXML()          {
                        TiXmlDocument doc;
                    	TiXmlElement * element = new TiXmlElement( "Parameter" );
                    	doc.LinkEndChild( element );

                        element->SetAttribute("name",_name.c_str());
                        element->SetDoubleAttribute("value",_value);
                        element->SetDoubleAttribute("min",_min);
                        element->SetDoubleAttribute("max",_max);
                        element->SetDoubleAttribute("step",_step);
                        //create a string with xml data
                        stringstream xmlString;
                        xmlString << doc;
                        return xmlString.str();
                    };
                    
        // bool setXML(std::string xmlString){
        //                 TiXmlDocument doc;
        //                 doc.Parse(xmlString.c_str());
        //                 cout << "parameter received xml: " << doc << endl;
        //                 return true;
        // }            
                    
    private:  
          double _value, _min, _max, _step;
          std::string _name;
    };


#endif //__PARAMETER_HPP__