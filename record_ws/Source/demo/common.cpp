#include "functions.h"
#include <fstream>
#include <iterator>
#include <cassert>
#include <iomanip>
#include <cstring>
#include <sstream>

#ifndef MOVI_C
#include <thread>
#include <chrono>

double now()
{
  return double(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()) * 1e-6;
}
#endif

// void save_features(std::string filename, Features const& xfeatures)
// {
//   std::ofstream file(filename);

//   file << xfeatures.range2.data[0].begin << " " << xfeatures.range2.data[0].end << " " << xfeatures.range2.data[1].begin << " " << xfeatures.range2.data[1].end << std::endl;

//   for(int i = 0 ; i < xfeatures.range2.data[1].end ; ++i)
//   {
//     Coordinate const* c = &xfeatures.pCoordinate[i];
//     file << c->x << " " << c->y << std::endl;
//   }

//   for(int i = 0 ; i < xfeatures.range2.data[1].end ; ++i)
//   {
//     Descriptor const* c = &xfeatures.pDescriptor[i];
//     std::copy(c->data,c->data+16,std::ostream_iterator<float>(file," "));
//     file << std::endl;
//   }
// }

// void save_enables(std::string filename, std::vector<bool> const& enables)
// {
//   std::ofstream file(filename);
//   file << enables.size() << std::endl;
//   std::copy(enables.begin(),enables.end(),std::ostream_iterator<bool>(file," "));
// }


// void save_xcoordinates(std::string filename, Coordinates& xcoordinates)
// {
//   std::ofstream file(filename);
//   assert(file);
//   file << xcoordinates.size << std::endl;
//   for(int i = 0 ; i < xcoordinates.size ; ++i)
//     file << xcoordinates.pCoordinate[i].x << " " << xcoordinates.pCoordinate[i].y << " ";
// }


// void save_xrange2(std::string filename, Range2 const& xrange2)
// {
//   std::ofstream file(filename);
//   assert(file);
//   file << xrange2.data[0].begin << " ";
//   file << xrange2.data[0].end << " ";
//   file << xrange2.data[1].begin << " "; 
//   file << xrange2.data[1].end;
// }


// void save_base(std::string filename, FloatImage const& base)
// {
//   float tmpfloat;

//   std::ofstream file(filename);
//   assert(file);
//   for(int i = 0 ; i < 16*169 ; ++i)
//   {
//     tmpfloat = ((float) base.ptr[i] +0.5f) / 32768.f;
//     file << tmpfloat << " ";
//   }
// }


// void save_ximage (std::string filename, CImage const& ximage) 
// {
//   int i;
//   std::ofstream file(filename);
//   assert(file);
//   file << ximage.rows << " " << ximage.cols << std::endl;
//   int nb_pixels = ximage.rows * ximage.cols;
//   for (i=0; i<nb_pixels; i++) {
//     file <<ximage.ptr[i]<< " ";
//   }
// }



// // void save_matches(std::string filename, XMatches const& matches)
// // {
// //   std::ofstream file(filename);
// //   assert(file);
// //   file << size(&matches) << std::endl;
// //   for(int i = 0 ; i< size(&matches) ; ++i)
// //     file
// //       << ((XMatch const*)cget(&matches,i))->first << " "
// //       << ((XMatch const*)cget(&matches,i))->second << std::endl;
// // }

// /*
// void save_matches_bin(std::string filename, XMatches const& matches)
// {
//   std::ofstream file(filename, std::ofstream::binary);
//   int sizei = size(&matches);
//   file.write((char*)&sizei, sizeof(int));
//   file.write((char*)matches.data, sizei * sizeof(XMatch));
//   file.close();
// }
// */
// void save_xdescriptors(std::string filename, Descriptors const& xdescriptors)
// {
//   std::ofstream file(filename);
//   assert(file);
//   file << xdescriptors.size << std::setprecision(16) << std::endl;
//   for(size_t i = 0 ; i < xdescriptors.size ; ++i)
//   {
//     Descriptor const* descriptor = &xdescriptors.pDescriptor[i];

//     for(int k = 0 ; k < DESCRIPTOR_LENGTH ; ++k)
//     {
//       file << descriptor->data[k] << " ";
//     }
//     file << std::endl;
//   }
// }


// void save_xdescriptors_bin(std::string filename, Descriptors const& xdescriptors)
// {
//   std::ofstream file(filename, std::ofstream::binary);
//   file.write((char*)&xdescriptors.size, sizeof(int));
//   file.write((char*)xdescriptors.pDescriptor, sizeof(float)*DESCRIPTOR_LENGTH*xdescriptors.size);
//   file.close();
// }



// #ifdef LOAD_BINARY_DATA
// void load_enables(std::string filename, std::vector<bool>& enables)
// {
//   std::ifstream file(filename, std::ifstream::binary);
//   int size;
//   file.read((char*)&size, sizeof(int));
//   char data;
//   for (int i=0; i<size; i++)
//   {
//     file.read(&data, sizeof(char));
//     enables.push_back((bool)data);
//   }
//   assert(enables.size()==(unsigned int)size);
//   file.close();
// }

// void load_features(std::string filename, Features& xfeatures)
// {
//   std::ifstream file(filename, std::ifstream::binary);
//   file.read((char*)&(xfeatures.range2), sizeof(Range2));
  
//   file.read((char*)&xfeatures.pCoordinate, sizeof(Coordinate) * xfeatures.range2.data[1].end );
//   file.read((char*)&xfeatures.pDescriptor, sizeof(Descriptor) * xfeatures.range2.data[1].end );

//   file.close();
// }

// /*
// void load_matches(std::string filename, XMatches& matches)
// {
//   std::ifstream file(filename, std::ifstream::binary);
//   int sizei;
//   file.read((char*)&sizei, sizeof(int));
//   XMatch data;
//   for (int i=0; i<sizei; i++)
//   {
//     file.read((char*)&data, sizeof(XMatch));
//     push_back(&matches,&data);
//   }
//   assert(size(&matches)==(unsigned int)sizei);
//   file.close();
// }
// */
// #else
// /*
// void load_enables(std::string filename, std::vector<bool>& enable)
// {
//   std::ifstream file(filename);
//   int size;
//   file >> size;
//   bool b;
//   while(file >> b) enable.push_back(b);
//   assert(int(enable.size())==size);
// }

// void load_features(std::string filename, XFeatures& xfeatures)
// {
//   std::ifstream file(filename);

//   file >> xfeatures.ranges.data[0].begin >> xfeatures.ranges.data[0].end >> xfeatures.ranges.data[1].begin >> xfeatures.ranges.data[1].end;

//   int x,y;
//   for(int i = 0 ; i < xfeatures.ranges.data[1].end; i++)
//   {
//     assert(file);
//     file >> x >> y;
//     XCoordinate coord{x,y};
//     push_back(&xfeatures.coordinates,&coord);
//   }

//   for(int i = 0 ; i < xfeatures.ranges.data[1].end; i++)
//   {
//     XDescriptor xdescriptor;
//     for(int k = 0 ; k < DESCRIPTOR_SIZE ; ++k)
//       file >> xdescriptor.data[k];
//     push_back(&xfeatures.descriptors,&xdescriptor);
//   }
// }

// void load_matches(std::string filename, XMatches& matches)
// {
//   std::ifstream file(filename);
//   int sizei;
//   file >> sizei;
//   int i,j;
//   while (file >> i >> j)
//   {
//     XMatch match;
//     match.first = i;
//     match.second = j;
//     push_back(&matches,&match);
//   }
//   assert(int(size(&matches)) == sizei);
// }

// // void save_matches(std::string filename, XMatches const& matches)
// // {
// //   std::ofstream file(filename);
// //   assert(file);
// //   file << size(&matches) << std::endl;
// //   for(int i = 0 ; i < matches.nb_element ; ++i)
// //     file << ((XMatch const*)cget(&matches,i))->first << " " << ((XMatch const*)cget(&matches,i))->second << std::endl;
// // }
// */
// #endif


// void check_outputs(std::string folder, std::string filename, std::string name)
// {
//   std::cout << " compare " << folder+filename << std::endl;
//   std::cout << " compare " << filename << std::endl;
//   std::stringstream str1,str2;
//   str1 << std::ifstream(folder+filename).rdbuf();
//   str2 << std::ifstream(filename).rdbuf();
//   std::cout << std::string(40,'*') << std::endl;
//   std::cout << " Check outputs of the " << name << std::endl;
//   if(str1.str() == str2.str())
//   {
//     std::cout << " " << name << " results are identical with the sample " << folder << std::endl;
//     std::cout << " -> SUCCESS " << std::endl;
//   }
//   else
//   {
//     std::cout << " Warning ! " << name << " results differ from the sample " << folder << std::endl;
//     std::cout << " -> FAILURE " << std::endl;
//     abort();
//   }
// }

// void check_outputs_bin(std::string folder, std::string filename, std::string name)
// {
//   std::stringstream str1,str2;
//   std::cout << " file " << folder+filename << std::endl;
//   std::cout << " file " << filename << std::endl;
//   str1 << std::ifstream(folder+filename,std::ios::binary).rdbuf();
//   str2 << std::ifstream(filename,std::ios::binary).rdbuf();
//   std::cout << std::string(40,'*') << std::endl;
//   std::cout << " Check outputs of the " << name << std::endl;

//   if(str1.str() == str2.str() && !str1.str().empty())
//   {
//     std::cout << " " << name << " results are identical with the sample " << folder << std::endl;
//     std::cout << " -> SUCCESS " << std::endl;
//   }
//   else
//   {
//     std::cout << " Warning ! " << name << " results differ from the sample " << folder << std::endl;
//     std::cout << " -> FAILURE " << std::endl;
//     std::cout << " File sizes: " << str1.str().size() << ", " << str2.str().size() << std::endl;
//     std::string s1 = str1.str();
//     std::string s2 = str2.str();
//     for(int i = 0 ; i < std::min(s1.size(),s2.size()); ++i)
//     {
//       if(s1[i]!=s2[i])
//       {
//         std::cout << " Files differ on byte " << i << " / " << std::min(s1.size(),s2.size()) << std::endl;
//         break;
//       }
//     }
//     abort();
//   }
// }
// /*
// void check_outputs_bin(std::string folder, std::string filename, std::string name)
// {
//   int size1, size2;
//   int match_flag;
//   int i;

//   char data1, data2;

//   std::ifstream file1(folder+filename, std::ifstream::binary);
//   std::ifstream file2(filename, std::ifstream::binary);
//   assert(file1 && "check_outputs_bin");assert(file2 && "check_outputs_bin");
//   file1.read((char*)&size1, sizeof(int));
//   file2.read((char*)&size2, sizeof(int));

//   std::cout << std::string(40,'*') << std::endl;
//   std::cout << " Binary Check outputs of the " << name << std::endl;
//   //std::cout << " Size are " << size1 << " and " << size2 << std::endl;

//   if (size1 != size2) {
//     std::cout << " Failure " << name << " Output size is different than " << folder << std::endl;
//   }

//   match_flag = 1;

//   for (i=0;i<size1;i++) {
//     file1.read(&data1, sizeof(char));
//     file2.read(&data2, sizeof(char));
//     if (data1 != data2) {
//         match_flag = 0;
//     }
//   }
   
   
//   if(match_flag)
//   {
//     std::cout << " " << name << " Binary results are identical with the sample " << folder << std::endl;
//     std::cout << " -> SUCCESS " << std::endl;
//   }
//   else
//   {
//     std::cout << " Warning ! " << name << " Binary results differ from the sample " << folder << std::endl;
//     std::cout << " -> FAILURE " << std::endl;
//     abort();
//   }




//   file1.close();
//   file2.close();


// }
// */




// std::string add_slash(std::string name)
// {
//   if(!name.empty())
//     if(name.back()!='/') name+="/";
//   return name;
// }

// void load_xcoordinates(std::string filename, Coordinates& xcoordinates)
// {
//   std::ifstream file(filename);
//   assert(file);
//   size_t sizei;
//   file >> sizei;
//   int x,y;
//   while(file >> x >> y)
//   {
//     xcoordinates.pCoordinate[xcoordinates.size].x = x;
//     xcoordinates.pCoordinate[xcoordinates.size].y = y;
//     xcoordinates.size++;
//   }
//   assert(xcoordinates.size==sizei);
// }



// void load_xframe(std::string filename1, std::string filename2, CFrame& xframe)
// {
//   std::array<std::string,2> filenames = {filename1,filename2};
//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     std::ifstream file(filenames[i]);
//     assert(file);
//     int rows,cols;
//     file >> rows >> cols;
    
//     int nb_pixels = rows * cols;
//     xframe.data[i] = create_ximage(cols,rows);

//     for(int k = 0 ; k < nb_pixels ; k++)
//     {
//       int v;
//       file >> v;
//       xat(xframe.data[i],k) = v;
//     }
//   }
// }

// void load_xrange2(std::string filename, Range2& xrange2)
// {
//   std::ifstream file(filename);
//   assert(file);
//   file >> xrange2.data[0].begin >> xrange2.data[0].end >> xrange2.data[1].begin >> xrange2.data[1].end;
// }

// void load_xdescriptors(std::string filename, Descriptors& xdescriptors)
// {
//   std::ifstream file(filename);
//   std::cout << " filename " << filename << std::endl;
//   assert(file);
//   size_t size;
//   file >> size;
//   //resize(&xdescriptors,size);
//   for(size_t i = 0 ; i < size ; ++i)
//   {
//     Descriptor descriptor;
//     for(size_t k = 0 ; k < DESCRIPTOR_LENGTH ; ++k)
//     {
//       assert(file);
//       file >> descriptor.data[k];
//     }
//     xdescriptors.pDescriptor[xdescriptors.size] = descriptor;
//     xdescriptors.size++;
//   }
// }

// // void save_xdescriptors(std::string filename, XDescriptors const& xdescriptors)
// // {
// //   std::ofstream file(filename);
// //   assert(file);
// //   file << size(&xdescriptors) << std::setprecision(16) << std::endl;
// //   for(size_t i = 0 ; i < size(&xdescriptors) ; ++i)
// //   {
// //     XDescriptor const* desc = (XDescriptor const*)cget(&xdescriptors,i);
// //     for(int k = 0 ; k < DESCRIPTOR_SIZE ; ++k)
// //     {
// //       file << desc->data[k] << " ";
// //     }
// //     file << std::endl;
// //   }
// // }


// void load_base(std::string filename, FloatImage & base)
// {
//   float tmpfloat;
//   base = create_xfloat_image(16,169);
  
//   std::ifstream file(filename);
//   assert(file);
//   for(int i = 0 ; i < 16*169 ; ++i)
//   {
//     file >> tmpfloat;
//     xat(base,i) =  tmpfloat * (1 << 15);
//   }
// }



// FloatImage create_xfloat_image(int cols, int rows)
// {
//   FloatImage ximage;
//   ximage.rows = rows;
//   ximage.cols = cols;

// #ifdef CPPARRAY
//   ximage.ptr.resize(rows*cols);
// #else
//   ximage.ptr = new s16[rows*cols];
// #endif
  
//   return ximage;
// }

// void free_xfloat_image(FloatImage & ximage)
// {
//   #ifdef CPPARRAY
//   std::vector<float>().swap(ximage.ptr);
//   #else
//   if (ximage.ptr) delete[] ximage.ptr;
//   ximage.ptr = 0;
//   #endif
// }

// void free_xframe(CFrame & xframe)
// {
//   free_ximage(xframe.data[0]);
//   free_ximage(xframe.data[1]);
// }




// CImage create_ximage(int cols, int rows)
// {
//   CImage ximage;
//   ximage.rows = rows;
//   ximage.cols = cols;

// #ifdef CPPARRAY
//   ximage.ptr.resize(rows*cols);
// #else
//   ximage.ptr = new uchar[rows*cols];
// #endif
  
//   return ximage;
// }


// uchar xat(CImage const& ximage, int x, int y)
// {
//   #ifdef CPPARRAY
//   return ximage.ptr.at(x+y*ximage.cols);
//   #else
//   assert(x+y*ximage.cols < ximage.cols*ximage.rows);
//   return ximage.ptr[x+y*ximage.cols];
//   #endif
// }




// uchar xat(CImage const& ximage, int x)
// {
// #ifdef CPPARRAY
//   return ximage.ptr.at(x);
// #else
//   assert(x < ximage.cols*ximage.rows);
//   return ximage.ptr[x];
// #endif
// }


// uchar& xat(CImage & ximage, int x)
// {
// #ifdef CPPARRAY
//   return ximage.ptr.at(x);
// #else
//   assert(x < ximage.cols*ximage.rows);
//   return ximage.ptr[x];
// #endif
// }


// s16 xat(FloatImage const& ximage, int x, int y)
// {
// #ifdef CPPARRAY
//   return ximage.ptr.at(x+y*ximage.cols);
// #else
//   assert(x+y*ximage.cols < ximage.cols*ximage.rows);
//   return ximage.ptr[x+y*ximage.cols];
// #endif
// }


// s16 & xat(FloatImage & ximage, int x, int y)
// {
// #ifdef CPPARRAY
//   return ximage.ptr.at(x+y*ximage.cols);
// #else
//   assert(x+y*ximage.cols < ximage.cols*ximage.rows);
//   return ximage.ptr[x+y*ximage.cols];
// #endif
// }

// s16 xat(FloatImage const& ximage, int x)
// {
// #ifdef CPPARRAY
//   return ximage.ptr.at(x);
// #else
//   assert(x < ximage.cols*ximage.rows);
//   return ximage.ptr[x];
// #endif
// }


// s16& xat(FloatImage & ximage, int x)
// {
// #ifdef CPPARRAY
//   return ximage.ptr.at(x);
// #else
//   assert(x < ximage.cols*ximage.rows);
//   return ximage.ptr[x];
// #endif
// }





// void free_ximage(CImage & ximage)
// {
// #ifdef CPPARRAY
//   std::vector<uchar>().swap(ximage.ptr);
// #else
//   if (ximage.ptr) delete[] ximage.ptr;
//   ximage.ptr = 0;
// #endif
// }

// /*
// XCoordinate const& xat(XCoordinates const& xcoordinates, int i)
// {
//   return xcoordinates.at(i);
// }

// XCoordinate & xat(XCoordinates & xcoordinates, int i)
// {
//   return xcoordinates.at(i);
// }


// XDescriptor const& xat(XDescriptors const& xdescriptors, int i)
// {
//   return xdescriptors.at(i);
// }

// XDescriptor & xat(XDescriptors & xdescriptors, int i)
// {
//   return xdescriptors.at(i);
// }
// */

// s16 xat(FloatImage13x13 const& image, int i)
// {
//   assert(i < _13x13);
//   return image.data[i];
// }

// s16 & xat(FloatImage13x13 & image, int i)
// {
//   assert(i < _13x13);
//   return image.data[i];
// }

// s16 xat(Descriptor const& xdescriptor, int i)
// {
//   assert(i < DESCRIPTOR_LENGTH);
//   return xdescriptor.data[i];
// }

// s16 & xat(Descriptor & xdescriptor, int i)
// {
//   assert(i < DESCRIPTOR_LENGTH);
//   return xdescriptor.data[i];
// }


// void load_dbg(DebugStruct* dbg, char* buffer)
// {
//   int sz = sizeof(Motion)+sizeof(IMUInfo)+sizeof(Metrics)+sizeof(XStereo)+sizeof(Range2);
//   char* start = buffer;

//   memcpy(dbg,buffer,sz);
//   buffer += sz;

//   // Coordinates
//   sz = 2*MAX_NUM_FEATURE*sizeof(Coordinate);
//   memcpy(dbg->coordinates,buffer,sz);
//   buffer += sz;

//   // Descriptors
//   sz = 2*MAX_NUM_FEATURE*sizeof(Descriptor);
//   memcpy(dbg->descriptors,buffer,sz);
//   buffer += sz;

//   // short nb_candidate1
//   sz = sizeof(dbg->nb_candidate1);
//   memcpy(&dbg->nb_candidate1,buffer,sz);
//   buffer += sz;

//   // match1_left.pData
//   sz = sizeof(dbg->match1_left.size);
//   memcpy(&dbg->match1_left.size,buffer,sz);
//   buffer += sz;

//   sz = MAX_NUM_FEATURE*sizeof(Match);
//   memcpy(dbg->match1_left.pData,buffer,sz);
//   buffer += sz;

//   // match1_right
//   sz = sizeof(dbg->match1_right.size);
//   memcpy(&dbg->match1_right.size,buffer,sz);
//   buffer += sz;

//   sz = MAX_NUM_FEATURE*sizeof(Match);
//   memcpy(dbg->match1_right.pData,buffer,sz);
//   buffer += sz;

//   // loc_output1
//   sz = sizeof(LocOutput);
//   memcpy(&dbg->loc_output1,buffer,sz);
//   buffer += sz;

//   // short nb_candidate2
//   sz = sizeof(dbg->nb_candidate2);
//   memcpy(&dbg->nb_candidate2,buffer,sz);
//   buffer += sz;

//   // int match2_left
//   sz = sizeof(dbg->match2_left.size);
//   memcpy(&dbg->match2_left.size,buffer,sz);
//   buffer += sz;

//   sz = MAX_NUM_FEATURE*sizeof(Match);
//   memcpy(dbg->match2_left.pData,buffer,sz);
//   buffer += sz;

//   // match2_right.size
//     // match1_left.pData
//   sz = sizeof(dbg->match2_right.size);
//   memcpy(&dbg->match2_right.size,buffer,sz);
//   buffer += sz;

//   sz = MAX_NUM_FEATURE*sizeof(Match);
//   memcpy(dbg->match2_right.pData,buffer,sz);
//   buffer += sz;

//   // loc_output2
//   sz = sizeof(LocOutput);
//   memcpy(&dbg->loc_output2,buffer,sz);
//   buffer += sz;

//   sz = sizeof(Range2);
//   memcpy(dbg->inliers_3d_range2.data,buffer,sz);
//   buffer += sz;

//   sz = 2*MAX_NUM_FEATURE*sizeof(XVec3);
//   memcpy(dbg->inliers_3d,buffer,sz);
//   buffer += sz;

//   sz = sizeof(DebugMap);
//   memcpy(&dbg->debug_map.nb_added_feature,buffer,sz);
//   buffer += sz;
// }



// void save_dbg(DebugStruct* dbg, char* dbg_buffer)
// {
//   memset(dbg_buffer,0,DebugStructSize);
//   char * buf = dbg_buffer;

//   int sz = sizeof(Motion);
//   memcpy(buf,&dbg->motion_data,sz);
//   buf += sz;

//   sz = sizeof(IMUInfo);
//   memcpy(buf,&dbg->imu_data,sz);
//   buf += sz;

//   sz = sizeof(Metrics);
//   memcpy(buf,&dbg->metrics_data,sz);
//   buf += sz;


//   sz = sizeof(XStereo);
//   memcpy(buf,&dbg->stereo,sz);
//   buf += sz;


//   sz = sizeof(Range2);
//   memcpy(buf,&dbg->coordinates_range2,sz);
//   buf += sz;

//   sz = dbg->coordinates_range2.data[1].end*sizeof(Coordinate);
//   memcpy(buf,dbg->coordinates,sz);
//   buf += 2*MAX_NUM_FEATURE*sizeof(Coordinate);

//   sz = dbg->coordinates_range2.data[1].end*sizeof(Descriptor);
//   memcpy(buf,dbg->descriptors,sz);
//   buf += 2*MAX_NUM_FEATURE*sizeof(Descriptor);

//   sz = sizeof(dbg->nb_candidate1);
//   memcpy(buf,&dbg->nb_candidate1,sz);
//   buf+=sz;

//   sz = sizeof(dbg->match1_left.size);
  
//   memcpy(buf,&dbg->match1_left.size,sz);
//   buf += sz;

//   sz = dbg->match1_left.size*sizeof(Match);
  
//   memcpy(buf,dbg->match1_left.pData,sz);
//   buf += MAX_NUM_FEATURE*sizeof(Match);


//   sz = sizeof(dbg->match1_right.size);
  
//   memcpy(buf,&dbg->match1_right.size,sz);
//   buf += sz;

//   sz = dbg->match1_right.size*sizeof(Match);
  
//   memcpy(buf,dbg->match1_right.pData,sz);
//   buf += MAX_NUM_FEATURE*sizeof(Match);

//   sz = sizeof(LocOutput);
  
//   memcpy(buf,&dbg->loc_output1,sz);
//   buf += sz;

//   sz = sizeof(dbg->nb_candidate2);
  
//   memcpy(buf,&dbg->nb_candidate2,sz);
//   buf += sz;

//   sz = sizeof(dbg->match2_left.size);
  
//   memcpy(buf,&dbg->match2_left.size,sz);
//   buf += sz;

//   sz = dbg->match2_left.size*sizeof(Match);
  
//   memcpy(buf,dbg->match2_left.pData,sz);
//   buf += MAX_NUM_FEATURE*sizeof(Match);


//   sz = sizeof(dbg->match2_right.size);
  
//   memcpy(buf,&dbg->match2_right.size,sz);
//   buf += sz;

//   sz = dbg->match2_right.size*sizeof(Match);
  
//   memcpy(buf,dbg->match2_right.pData,sz);
//   buf += MAX_NUM_FEATURE*sizeof(Match);

//   sz = sizeof(LocOutput);
  
//   memcpy(buf,&dbg->loc_output2,sz);
//   buf += sz;

//   sz = sizeof(Range2);
//   memcpy(buf,dbg->inliers_3d_range2.data,sz);
//   buf += sz;

//   sz = dbg->inliers_3d_range2.data[1].end*sizeof(XVec3);
//   memcpy(buf,dbg->inliers_3d,sz);
//   buf += 2*MAX_NUM_FEATURE*sizeof(XVec3);

//   sz = sizeof(DebugMap);
//   memcpy(buf,&dbg->debug_map.nb_added_feature,sz);
//   buf += sz;
// }




// //#define ABORT_IF_DIFF
  

// void _abort_if_diff()
// {
//   #ifdef ABORT_IF_DIFF
//   std::cout << " Abort because a difference is found" << std::endl;
//   abort();
//   #endif
// }

 
// //#define DBG_INFO_DISP_ALL



// int diff_or_disp_short(short a, short b)
// {
//   #ifdef DBG_INFO_DISP_ALL
//     return 1;
//   #else
//     return a != b;
//   #endif
// }

// int diff_or_disp_float(float a, float b)
// {
//   #ifdef DBG_INFO_DISP_ALL
//     return 1;
//   #else
//     return a != b;
//   #endif
// }

// int diff_or_disp_int(int a, int b)
// {
//   #ifdef DBG_INFO_DISP_ALL
//     return 1;
//   #else
//     return a != b;
//   #endif
// }

// int diff_or_disp_u16(u16 a, u16 b)
// {
//   #ifdef DBG_INFO_DISP_ALL
//     return 1;
//   #else
//     return a != b;
//   #endif
// }

// void check_debug_struct(DebugStructMCCI* movi_dbg1, DebugStructMCCI* pc_dbg2, int verbose_level, std::ostream& out=std::cout)
// {
//   if (verbose_level==CHECK_VERBOSE_OFF) return;

//   if(diff_or_disp_short(movi_dbg1->motion_data.device_state,pc_dbg2->motion_data.device_state))
//   {
//     out << " device_state " << " " << movi_dbg1->motion_data.device_state << " " << pc_dbg2->motion_data.device_state << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->motion_data.map_state,pc_dbg2->motion_data.map_state))
//   {
//     out << " map_state " << " " << movi_dbg1->motion_data.map_state << " " << pc_dbg2->motion_data.map_state << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->motion_data.time,pc_dbg2->motion_data.time))
//   {
//     out << " time " << " " << movi_dbg1->motion_data.time << " " << pc_dbg2->motion_data.time << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->motion_data.frameNo,pc_dbg2->motion_data.frameNo))
//   {
//     out << " frameNo " << " " << movi_dbg1->motion_data.frameNo << " " << pc_dbg2->motion_data.frameNo << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < 9 ; ++i)
//     if(diff_or_disp_float(movi_dbg1->motion_data.pose_rotation[i],pc_dbg2->motion_data.pose_rotation[i]))
//     {
//       out << "R " << " " << i << " " << movi_dbg1->motion_data.pose_rotation[i] << " " << pc_dbg2->motion_data.pose_rotation[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }

//   for(int i = 0 ; i < 3 ; ++i)
//     if(diff_or_disp_float(movi_dbg1->motion_data.pose_translation[i],pc_dbg2->motion_data.pose_translation[i]))
//     {
//       out << "T " << " " << i << " " << movi_dbg1->motion_data.pose_translation[i] << " " << pc_dbg2->motion_data.pose_translation[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }

//   for(int i = 0 ; i < 3 ; ++i)
//     if(diff_or_disp_float(movi_dbg1->motion_data.pose_angle[i],pc_dbg2->motion_data.pose_angle[i]))
//     {
//       out << "Degree " << " " << i << " " << movi_dbg1->motion_data.pose_angle[i] << " " << pc_dbg2->motion_data.pose_angle[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }


//   // for(int i = 0 ; i < 3 ; ++i)
//   // {
//   //   if(diff_or_disp_float(movi_dbg1->imu_data.accelData[i],pc_dbg2->imu_data.accelData[i]))
//   //   {
//   //     out << "IMU Info %d %.2f %.2f \n",i,movi_dbg1->imu_data.accelData[i],pc_dbg2->imu_data.accelData[i] << std::endl;
//   //     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//   //       break;
//   //     _abort_if_diff();
//   //   }
//   // }


//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_features_left,pc_dbg2->metrics_data.nb_features_left))
//   {
//     out << " nb_features_left " << " " << movi_dbg1->metrics_data.nb_features_left << " " << pc_dbg2->metrics_data.nb_features_left << std::endl;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_features_right,pc_dbg2->metrics_data.nb_features_right))
//   {
//     out << " nb_features_right " << " " << movi_dbg1->metrics_data.nb_features_right << " " << pc_dbg2->metrics_data.nb_features_right << std::endl;
//     _abort_if_diff();
//   }


//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc1_left,pc_dbg2->metrics_data.nb_matches_loc1_left))
//   {
//     out << " nb_matches_loc1_left " << " " << movi_dbg1->metrics_data.nb_matches_loc1_left << " " << pc_dbg2->metrics_data.nb_matches_loc1_left << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc1_right,pc_dbg2->metrics_data.nb_matches_loc1_right))
//   {
//     out << " nb_matches_loc1_right " << " " << movi_dbg1->metrics_data.nb_matches_loc1_right << " " << pc_dbg2->metrics_data.nb_matches_loc1_right << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc2_left,pc_dbg2->metrics_data.nb_matches_loc2_left))
//   {
//     out << " nb_matches_loc2_left " << " " << movi_dbg1->metrics_data.nb_matches_loc2_left << " " << pc_dbg2->metrics_data.nb_matches_loc2_left << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc2_right,pc_dbg2->metrics_data.nb_matches_loc2_right))
//   {
//     out << " nb_matches_loc2_right " << " " << movi_dbg1->metrics_data.nb_matches_loc2_right << " " << pc_dbg2->metrics_data.nb_matches_loc2_right << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_inliers,pc_dbg2->metrics_data.nb_inliers))
//   {
//     _abort_if_diff();
//     out << " nb_inliers " << " " << movi_dbg1->metrics_data.nb_inliers << " " << pc_dbg2->metrics_data.nb_inliers << std::endl;
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_local_map_size,pc_dbg2->metrics_data.nb_local_map_size))
//   {
//     out << " nb_local_map_size " << " " << movi_dbg1->metrics_data.nb_local_map_size << " " << pc_dbg2->metrics_data.nb_local_map_size << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_map_keyframe,pc_dbg2->metrics_data.nb_map_keyframe))
//   {
//     out << " nb_map_keyframe " << " " << movi_dbg1->metrics_data.nb_map_keyframe << " " << pc_dbg2->metrics_data.nb_map_keyframe << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->metrics_data.nb_map_features,pc_dbg2->metrics_data.nb_map_features))
//   {
//     out << " nb_map_features " << " " << movi_dbg1->metrics_data.nb_map_features << " " << pc_dbg2->metrics_data.nb_map_features << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->metrics_data.nb_map_3D,pc_dbg2->metrics_data.nb_map_3D))
//   {
//     out << " nb_map_3D " << " " << movi_dbg1->metrics_data.nb_map_3D << " " << pc_dbg2->metrics_data.nb_map_3D << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < 9+3+7 ; ++i)
//   {
//     float* f1 = (float*)&movi_dbg1->stereo;
//     float* f2 = (float*)&pc_dbg2->stereo;
//     if(diff_or_disp_float(f1[i],f2[i]))
//     {
//       out << " stereo["<<i<< "]" << f1[i] << " " << f2[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }


//   if(
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[0].begin,pc_dbg2->coordinates_range2.data[0].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[0].end,pc_dbg2->coordinates_range2.data[0].end)
//       ||
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[1].begin,pc_dbg2->coordinates_range2.data[1].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[1].end,pc_dbg2->coordinates_range2.data[1].end)
//     )
//   {
//     out << " coordinates_range2 1: " 
//       << movi_dbg1->coordinates_range2.data[0].begin << " " << movi_dbg1->coordinates_range2.data[0].end << " " << 
//       movi_dbg1->coordinates_range2.data[1].begin << " " << movi_dbg1->coordinates_range2.data[1].end << std::endl;
//     out << " coordinates_range2 2: "
//       << pc_dbg2->coordinates_range2.data[0].begin << " " << pc_dbg2->coordinates_range2.data[0].end << " " << 
//       pc_dbg2->coordinates_range2.data[1].begin << " " << pc_dbg2->coordinates_range2.data[1].end << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = movi_dbg1->coordinates_range2.data[0].begin ; i < movi_dbg1->coordinates_range2.data[1].end ; ++i)
//   {
//     if(
//         diff_or_disp_u16(movi_dbg1->coordinates[i].x,pc_dbg2->coordinates[i].x)
//         ||
//         diff_or_disp_u16(movi_dbg1->coordinates[i].y,pc_dbg2->coordinates[i].y)
//       )
//     {
//         out
//           << "Coordinate[" << i
//           << "]: x1:" << movi_dbg1->coordinates[i].x 
//           << " y1:" << movi_dbg1->coordinates[i].y
//           << " x2:" << pc_dbg2->coordinates[i].x
//           << " y2:" << pc_dbg2->coordinates[i].y << std::endl;
//         if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//           break;

//       _abort_if_diff();
//     }

//     for(int k = 0 ; k < 16 ; ++k)
//     {
//       if(diff_or_disp_short(movi_dbg1->descriptors[i].data[k],pc_dbg2->descriptors[i].data[k]))
//       {
//         out << "Descriptor[" << i << "][" << k << "]:" << movi_dbg1->descriptors[i].data[k] << " " << pc_dbg2->descriptors[i].data[k] << std::endl;
//         if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//           break;
//         _abort_if_diff();
//       }

//     }
//   }


//   for(int i = movi_dbg1->coordinates_range2.data[0].begin ; i < movi_dbg1->coordinates_range2.data[1].end ; ++i)
//   {
//     bool ok = false;
//     for(int j = pc_dbg2->coordinates_range2.data[0].begin ; j < pc_dbg2->coordinates_range2.data[1].end ; ++j)
//     {
//       if(
//           (movi_dbg1->coordinates[i].x==pc_dbg2->coordinates[j].x)
//           &&
//           (movi_dbg1->coordinates[i].y==pc_dbg2->coordinates[j].y)
//         )
//       {
//         ok = true;
//       }
//     }
//     if(!ok)
//     {
//       out << "!!! Features mismatches !!!\n" << std::endl;
//       goto break_feature_comparison;
//     }
//   }
//   break_feature_comparison:


//   if(diff_or_disp_short(movi_dbg1->nb_candidate1,pc_dbg2->nb_candidate1))
//   {
//     out << " nb_candidate1 " << " " << movi_dbg1->nb_candidate1 << " " << pc_dbg2->nb_candidate1 << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->match1_left.size,pc_dbg2->match1_left.size))
//   {
//     out << " match1_left.size " << " " << movi_dbg1->match1_left.size << " " << pc_dbg2->match1_left.size << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < movi_dbg1->match1_left.size ; ++i)
//   {
//     if (
//         diff_or_disp_u16(movi_dbg1->match1_left.pData[i].first,pc_dbg2->match1_left.pData[i].first)
//         ||
//         diff_or_disp_u16(movi_dbg1->match1_left.pData[i].second,pc_dbg2->match1_left.pData[i].second)
//       )
//     {
//       out << "match1_left first[ " << i << "]:"
//       << " first: " << movi_dbg1->match1_left.pData[i].first
//       << " " << pc_dbg2->match1_left.pData[i].first
//       << " second: " << movi_dbg1->match1_left.pData[i].second
//       << " " << pc_dbg2->match1_left.pData[i].second
//       << std::endl;

//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }


//   if(diff_or_disp_int(movi_dbg1->match1_right.size,pc_dbg2->match1_right.size))
//   {
//     out << " match1_right.size " << " " << movi_dbg1->match1_right.size << " " << pc_dbg2->match1_right.size << std::endl;
//     _abort_if_diff();
//   }


//   for(int i = 0 ; i < movi_dbg1->match1_right.size ; ++i)
//   {
//     if (
//         diff_or_disp_u16(movi_dbg1->match1_right.pData[i].first,pc_dbg2->match1_right.pData[i].first)
//         ||
//         diff_or_disp_u16(movi_dbg1->match1_right.pData[i].second,pc_dbg2->match1_right.pData[i].second)
//       )
//     {
//       out << "match1_right first[" << i << "]: first:"
//       << movi_dbg1->match1_right.pData[i].first << " " << pc_dbg2->match1_right.pData[i].first
//       << " second "
//       << movi_dbg1->match1_right.pData[i].second << " " << pc_dbg2->match1_right.pData[i].second
//       << std::endl;

//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output1.C,pc_dbg2->loc_output1.C))
//   {
//     out << "loc_output1.C " << " " << movi_dbg1->loc_output1.C << " " << pc_dbg2->loc_output1.C << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   for(int k = 0 ; k < 6 ; ++k)
//   {
//     if(diff_or_disp_float(movi_dbg1->loc_output1.C,pc_dbg2->loc_output1.C))
//     {
//       out << "loc_output1.delta[" << k << "] " << movi_dbg1->loc_output1.C << " "<< pc_dbg2->loc_output1.C << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//       _abort_if_diff();
//     }
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output1.lambda,pc_dbg2->loc_output1.lambda))
//   {
//     out << "loc_output1.lambda " << " " << movi_dbg1->loc_output1.lambda << " " << pc_dbg2->loc_output1.lambda << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output1.error_before_opt,pc_dbg2->loc_output1.error_before_opt))
//   {
//     out << "loc_output1.error_before_opt " << " " << movi_dbg1->loc_output1.error_before_opt << " " << pc_dbg2->loc_output1.error_before_opt << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_float(movi_dbg1->loc_output1.curr,pc_dbg2->loc_output1.curr))
//   {
//     out << "loc_output1.curr " << " " << movi_dbg1->loc_output1.curr << " " << pc_dbg2->loc_output1.curr << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output1.rho,pc_dbg2->loc_output1.rho))
//   {
//     out << "loc_output1.rho " << " " << movi_dbg1->loc_output1.rho << " " << pc_dbg2->loc_output1.rho << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output1.alpha,pc_dbg2->loc_output1.alpha))
//   {
//     out << "loc_output1.alpha " << " " << movi_dbg1->loc_output1.alpha << " " << pc_dbg2->loc_output1.alpha << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->loc_output1.printAlpha,pc_dbg2->loc_output1.printAlpha))
//   {
//     out << "loc_output1.printAlpha " << " " << movi_dbg1->loc_output1.printAlpha << " " << pc_dbg2->loc_output1.printAlpha << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   end_of_loc_output1:

//   if(diff_or_disp_short(movi_dbg1->nb_candidate2,pc_dbg2->nb_candidate2))
//   {
//     out << " nb_candidate2 " << " " << movi_dbg1->nb_candidate2 << " " << pc_dbg2->nb_candidate2 << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->match2_left.size,pc_dbg2->match2_left.size))
//   {
//     out << " match2_left.size "  << " " << movi_dbg1->match2_left.size << " " << pc_dbg2->match2_left.size << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < movi_dbg1->match2_left.size ; ++i)
//   {
//     if (
//         diff_or_disp_u16(movi_dbg1->match2_left.pData[i].first,pc_dbg2->match2_left.pData[i].first)
//         ||
//         diff_or_disp_u16(movi_dbg1->match2_left.pData[i].second,pc_dbg2->match2_left.pData[i].second)
//       )
//     {
//       out << "match2_left first[" << i
//         << "]: first :"
//         << movi_dbg1->match2_left.pData[i].first << " " << pc_dbg2->match2_left.pData[i].first
//         << " second " <<  movi_dbg1->match2_left.pData[i].second << " " << pc_dbg2->match2_left.pData[i].second
//         << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }


//   if(diff_or_disp_int(movi_dbg1->match2_right.size,pc_dbg2->match2_right.size))
//   {
//     out << " match2_right.size " << " " << movi_dbg1->match2_right.size << " " << pc_dbg2->match2_right.size << std::endl;
//     _abort_if_diff();
//   }


//   for(int i = 0 ; i < movi_dbg1->match2_right.size ; ++i)
//   {
//     if (
//         diff_or_disp_u16(movi_dbg1->match2_right.pData[i].first,pc_dbg2->match2_right.pData[i].first)
//         ||
//         diff_or_disp_u16(movi_dbg1->match2_right.pData[i].second,pc_dbg2->match2_right.pData[i].second)
//       )
//     {
//       out << "match2_right first[" << i << "]: first: " <<
//         movi_dbg1->match2_right.pData[i].first << " " << pc_dbg2->match2_right.pData[i].first << " second " << 
//         movi_dbg1->match2_right.pData[i].second << " " << pc_dbg2->match2_right.pData[i].second
//         << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output2.C,pc_dbg2->loc_output2.C))
//   {
//     out << "loc_output2.C " << " " << movi_dbg1->loc_output2.C << " " << pc_dbg2->loc_output2.C << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//       goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   for(int k = 0 ; k < 6 ; ++k)
//   {
//     if(diff_or_disp_float(movi_dbg1->loc_output2.C,pc_dbg2->loc_output2.C))
//     {
//       out << "loc_output2.delta[" << k << "] " << movi_dbg1->loc_output2.C << " " << pc_dbg2->loc_output2.C << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//       _abort_if_diff();
//     }
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output2.lambda,pc_dbg2->loc_output2.lambda))
//   {
//     out << "loc_output2.lambda " << " " << movi_dbg1->loc_output2.lambda << " " << pc_dbg2->loc_output2.lambda << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output2.error_before_opt,pc_dbg2->loc_output2.error_before_opt))
//   {
//     out << "loc_output2.error_before_opt " << " " << movi_dbg1->loc_output2.error_before_opt << " " << pc_dbg2->loc_output2.error_before_opt << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_float(movi_dbg1->loc_output2.curr,pc_dbg2->loc_output2.curr))
//   {
//     out << "loc_output2.curr " << " " << movi_dbg1->loc_output2.curr << " " << pc_dbg2->loc_output2.curr << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output2.rho,pc_dbg2->loc_output2.rho))
//   {
//     out << "loc_output2.rho " << " " << movi_dbg1->loc_output2.rho << " " << pc_dbg2->loc_output2.rho << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output2.alpha,pc_dbg2->loc_output2.alpha))
//   {
//     out << "loc_output2.alpha " << " " << movi_dbg1->loc_output2.alpha << " " << pc_dbg2->loc_output2.alpha << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->loc_output2.printAlpha,pc_dbg2->loc_output2.printAlpha))
//   {
//     out << "loc_output2.printAlpha " << " " << movi_dbg1->loc_output2.printAlpha << " " << pc_dbg2->loc_output2.printAlpha << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   end_of_loc_output2:

//   int nb_inlier_print = 0;

//   if(
//       diff_or_disp_int(movi_dbg1->inliers_3d_range2.data[0].begin,pc_dbg2->inliers_3d_range2.data[0].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->inliers_3d_range2.data[0].end,pc_dbg2->inliers_3d_range2.data[0].end)
//       ||
//       diff_or_disp_int(movi_dbg1->inliers_3d_range2.data[1].begin,pc_dbg2->inliers_3d_range2.data[1].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->inliers_3d_range2.data[1].end,pc_dbg2->inliers_3d_range2.data[1].end)
//     )
//   {
//     out << " inliers_3d_range2 1: " <<
//       movi_dbg1->inliers_3d_range2.data[0].begin << " " << movi_dbg1->inliers_3d_range2.data[0].end << " " << 
//       movi_dbg1->inliers_3d_range2.data[1].begin << " " << movi_dbg1->inliers_3d_range2.data[1].end << std::endl;
//     out << " inliers_3d_range2 2: " <<
//       pc_dbg2->inliers_3d_range2.data[0].begin << " " << pc_dbg2->inliers_3d_range2.data[0].end << " " << 
//       pc_dbg2->inliers_3d_range2.data[1].begin << " " << pc_dbg2->inliers_3d_range2.data[1].end << std::endl;
//     _abort_if_diff();
//   }
//   else {
//      nb_inlier_print = pc_dbg2->inliers_3d_range2.data[1].end;
//   }

//   for(int i = 0; i < nb_inlier_print ; ++i)
//   {
//     if(
//         diff_or_disp_float(movi_dbg1->inliers_3d[i].data[0],pc_dbg2->inliers_3d[i].data[0])
//         ||
//         diff_or_disp_float(movi_dbg1->inliers_3d[i].data[1],pc_dbg2->inliers_3d[i].data[1])
//         ||
//         diff_or_disp_float(movi_dbg1->inliers_3d[i].data[2],pc_dbg2->inliers_3d[i].data[2])
//       )
//     {
//       out << "XVec3[" << i
//         << "]: x1:"
//         << movi_dbg1->inliers_3d[i].data[0]
//         << " x2:" << 
//         pc_dbg2->inliers_3d[i].data[0]
//         << " y1:" <<
//         movi_dbg1->inliers_3d[i].data[1]
//         << " y2:" <<
//         pc_dbg2->inliers_3d[i].data[1]
//         << " z1:" <<
//         movi_dbg1->inliers_3d[i].data[2]
//         << " z2:" <<
//         pc_dbg2->inliers_3d[i].data[2]
//         << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }

//   if(diff_or_disp_short(movi_dbg1->debug_map.nb_added_feature,pc_dbg2->debug_map.nb_added_feature))
//   {
//     out << "debug map nb_added_feature " << " " << movi_dbg1->debug_map.nb_added_feature << " " << pc_dbg2->debug_map.nb_added_feature << std::endl;
//     _abort_if_diff();
//   }


//   for(int i = 0 ; i < 9 ; ++i)
//   if(diff_or_disp_float(movi_dbg1->debug_map.added_pose.rotation.data[i],pc_dbg2->debug_map.added_pose.rotation.data[i]))
//   {
//     out << "debug map added_pose "<< " " << i<< " " << movi_dbg1->debug_map.added_pose.rotation.data[i]<< " " << pc_dbg2->debug_map.added_pose.rotation.data[i] << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//     _abort_if_diff();
//   }


//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_1.radius_x,pc_dbg2->debug_map.match_config_1.radius_x))
//   {
//     out << "debug map match_config_1.radius_x "<<  movi_dbg1->debug_map.match_config_1.radius_x<< " " << pc_dbg2->debug_map.match_config_1.radius_x << std::endl;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_1.radius_y,pc_dbg2->debug_map.match_config_1.radius_y))
//   {
//     out << "debug map match_config_1.radius_y "<<  movi_dbg1->debug_map.match_config_1.radius_y<< " " << pc_dbg2->debug_map.match_config_1.radius_y << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->debug_map.match_config_1.score_min,pc_dbg2->debug_map.match_config_1.score_min))
//   {
//     out << "debug map match_config_1.score_min "<<  movi_dbg1->debug_map.match_config_1.score_min<< " " << pc_dbg2->debug_map.match_config_1.score_min << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_enable1_with_previous[i],pc_dbg2->debug_map.nb_enable1_with_previous[i]))
//     {
//       out << "debug map nb_enable1_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_enable1_with_previous[i] << " " << pc_dbg2->debug_map.nb_enable1_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_projected_with_previous[i],pc_dbg2->debug_map.nb_projected_with_previous[i]))
//     {
//       out << "debug map nb_projected_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_projected_with_previous[i] << " " << pc_dbg2->debug_map.nb_projected_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_match1_with_previous[i],pc_dbg2->debug_map.nb_match1_with_previous[i]))
//     {
//       out << "debug map nb_match1_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_match1_with_previous[i] << " " << pc_dbg2->debug_map.nb_match1_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_tracked_with_previous[i],pc_dbg2->debug_map.nb_tracked_with_previous[i]))
//     {
//       out << "debug map nb_tracked_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_tracked_with_previous[i] << " " << pc_dbg2->debug_map.nb_tracked_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }


//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_2.radius_x,pc_dbg2->debug_map.match_config_2.radius_x))
//   {
//     out << "debug map match_config_2.radius_x "<<  movi_dbg1->debug_map.match_config_2.radius_x<< " " << pc_dbg2->debug_map.match_config_2.radius_x << std::endl;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_2.radius_y,pc_dbg2->debug_map.match_config_2.radius_y))
//   {
//     out << "debug map match_config_2.radius_y "<<  movi_dbg1->debug_map.match_config_2.radius_y<< " " << pc_dbg2->debug_map.match_config_2.radius_y << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->debug_map.match_config_2.score_min,pc_dbg2->debug_map.match_config_2.score_min))
//   {
//     out << "debug map match_config_2.score_min "<<  movi_dbg1->debug_map.match_config_2.score_min<< " " << pc_dbg2->debug_map.match_config_2.score_min << std::endl;
//     _abort_if_diff();
//   }

  

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_enable2_with_previous[i],pc_dbg2->debug_map.nb_enable2_with_previous[i]))
//     {
//       out << "debug map nb_enable2_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_enable2_with_previous[i] << " " << pc_dbg2->debug_map.nb_enable2_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }


//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_match2_with_previous[i],pc_dbg2->debug_map.nb_match2_with_previous[i]))
//     {
//       out << "debug map nb_match2_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_match2_with_previous[i] << " " << pc_dbg2->debug_map.nb_match2_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_new3d_with_previous[i],pc_dbg2->debug_map.nb_new3d_with_previous[i]))
//     {
//       out << "debug map nb_new3d_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_new3d_with_previous[i] << " " << pc_dbg2->debug_map.nb_new3d_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }


//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_3.radius_x,pc_dbg2->debug_map.match_config_3.radius_x))
//   {
//     out << "debug map match_config_3.radius_x "<<  movi_dbg1->debug_map.match_config_3.radius_x<< " " << pc_dbg2->debug_map.match_config_3.radius_x << std::endl;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_3.radius_y,pc_dbg2->debug_map.match_config_3.radius_y))
//   {
//     out << "debug map match_config_3.radius_y "<<  movi_dbg1->debug_map.match_config_3.radius_y<< " " << pc_dbg2->debug_map.match_config_3.radius_y << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->debug_map.match_config_3.score_min,pc_dbg2->debug_map.match_config_3.score_min))
//   {
//     out << "debug map match_config_3.score_min "<<  movi_dbg1->debug_map.match_config_3.score_min<< " " << pc_dbg2->debug_map.match_config_3.score_min << std::endl;
//     _abort_if_diff();
//   }


//   if(diff_or_disp_int(movi_dbg1->debug_map.nb_match_with_stereo_matching,pc_dbg2->debug_map.nb_match_with_stereo_matching))
//   {
//     out << "debug map nb_match_with_stereo_matching "<<  movi_dbg1->debug_map.nb_match_with_stereo_matching << " " << pc_dbg2->debug_map.nb_match_with_stereo_matching << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->debug_map.nb_new3d_with_stereo,pc_dbg2->debug_map.nb_new3d_with_stereo))
//   {
//     out << "debug map nb_new3d_with_stereo "<<  movi_dbg1->debug_map.nb_new3d_with_stereo << " " << pc_dbg2->debug_map.nb_new3d_with_stereo << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->debug_map.nb_observation,pc_dbg2->debug_map.nb_observation))
//   {
//     out << "debug map nb_observation "<<  movi_dbg1->debug_map.nb_observation << " " << pc_dbg2->debug_map.nb_observation << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->debug_map.nb_opt_pose,pc_dbg2->debug_map.nb_opt_pose))
//   {
//     out << "debug map nb_opt_pose "<<  movi_dbg1->debug_map.nb_opt_pose << " " << pc_dbg2->debug_map.nb_opt_pose << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->debug_map.n_iter,pc_dbg2->debug_map.n_iter))
//   {
//     out << "debug map n_iter "<<  movi_dbg1->debug_map.n_iter << " " << pc_dbg2->debug_map.n_iter << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->debug_map.error,pc_dbg2->debug_map.error))
//   {
//     out << "debug map error "<<  movi_dbg1->debug_map.error << " " << pc_dbg2->debug_map.error << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->debug_map.cr,pc_dbg2->debug_map.cr))
//   {
//     out << "debug map cr "<<  movi_dbg1->debug_map.cr << " " << pc_dbg2->debug_map.cr << std::endl;
//     _abort_if_diff();
//   }

//   if(
//       diff_or_disp_int(movi_dbg1->debug_map.local_map_range.data[0].begin,pc_dbg2->debug_map.local_map_range.data[0].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->debug_map.local_map_range.data[0].end,pc_dbg2->debug_map.local_map_range.data[0].end)
//       ||
//       diff_or_disp_int(movi_dbg1->debug_map.local_map_range.data[1].begin,pc_dbg2->debug_map.local_map_range.data[1].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->debug_map.local_map_range.data[1].end,pc_dbg2->debug_map.local_map_range.data[1].end)
//     )
//   {
//     out << " inliers_3d_range2 1: " <<
//       movi_dbg1->debug_map.local_map_range.data[0].begin << " " << movi_dbg1->debug_map.local_map_range.data[0].end << " " << 
//       movi_dbg1->debug_map.local_map_range.data[1].begin << " " << movi_dbg1->debug_map.local_map_range.data[1].end << std::endl;
//     out << " inliers_3d_range2 2: " <<
//       pc_dbg2->debug_map.local_map_range.data[0].begin << " " << pc_dbg2->debug_map.local_map_range.data[0].end << " " << 
//       pc_dbg2->debug_map.local_map_range.data[1].begin << " " << pc_dbg2->debug_map.local_map_range.data[1].end << std::endl;
//     _abort_if_diff();
//   }


//   if(
//       diff_or_disp_float(movi_dbg1->debug_map.first_3d.data[0],pc_dbg2->debug_map.first_3d.data[0])
//       ||
//       diff_or_disp_float(movi_dbg1->debug_map.first_3d.data[1],pc_dbg2->debug_map.first_3d.data[1])
//       ||
//       diff_or_disp_float(movi_dbg1->debug_map.first_3d.data[2],pc_dbg2->debug_map.first_3d.data[2])
//     )
//   {
//     out << "First p3d of the map "
//       << ": x1:"
//       << movi_dbg1->debug_map.first_3d.data[0]
//       << " x2:" << 
//       pc_dbg2->debug_map.first_3d.data[0]
//       << " y1:" <<
//       movi_dbg1->debug_map.first_3d.data[1]
//       << " y2:" <<
//       pc_dbg2->debug_map.first_3d.data[1]
//       << " z1:" <<
//       movi_dbg1->debug_map.first_3d.data[2]
//       << " z2:" <<
//       pc_dbg2->debug_map.first_3d.data[2]
//       << std::endl;
//     _abort_if_diff();
//   }


//   if(
//       diff_or_disp_float(movi_dbg1->debug_map.last_3d.data[0],pc_dbg2->debug_map.last_3d.data[0])
//       ||
//       diff_or_disp_float(movi_dbg1->debug_map.last_3d.data[1],pc_dbg2->debug_map.last_3d.data[1])
//       ||
//       diff_or_disp_float(movi_dbg1->debug_map.last_3d.data[2],pc_dbg2->debug_map.last_3d.data[2])
//     )
//   {
//     out << "First p3d of the map "
//       << ": x1:"
//       << movi_dbg1->debug_map.last_3d.data[0]
//       << " x2:" << 
//       pc_dbg2->debug_map.last_3d.data[0]
//       << " y1:" <<
//       movi_dbg1->debug_map.last_3d.data[1]
//       << " y2:" <<
//       pc_dbg2->debug_map.last_3d.data[1]
//       << " z1:" <<
//       movi_dbg1->debug_map.last_3d.data[2]
//       << " z2:" <<
//       pc_dbg2->debug_map.last_3d.data[2]
//       << std::endl;
//     _abort_if_diff();
//   }


//   float pc_size_of_tmp_buff = pc_dbg2->tmpBuff[0];
//   float movi_size_of_tmp_buff = movi_dbg1->tmpBuff[0];
//   if(pc_size_of_tmp_buff!=movi_size_of_tmp_buff)
//   {
//     out << " movi_size_of_tmp_buff!=pc_size_of_tmp_buff " << movi_size_of_tmp_buff << " != " << pc_size_of_tmp_buff << std::endl;
//     int size = std::min(pc_size_of_tmp_buff,movi_size_of_tmp_buff);
//     for(int i = 0 ; i < size ; ++i)
//     {
//       if(pc_dbg2->tmpBuff[i]!=movi_dbg1->tmpBuff[i])
//       {
//         out << "movi tmpBuff[" << i <<"] != pc tmpBuff[" << i << "]: " << movi_dbg1->tmpBuff[i] << " != " << pc_dbg2->tmpBuff[i] << std::endl;
//       }
//     }
//   }
// }
// void check_debug_struct(DebugStruct* movi_dbg1, DebugStruct* pc_dbg2, int verbose_level, std::ostream& out)
// {
//   if (verbose_level==CHECK_VERBOSE_OFF) return;


//   if(diff_or_disp_short(movi_dbg1->motion_data.device_state,pc_dbg2->motion_data.device_state))
//   {
//     out << " device_state " << " " << movi_dbg1->motion_data.device_state << " " << pc_dbg2->motion_data.device_state << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->motion_data.map_state,pc_dbg2->motion_data.map_state))
//   {
//     out << " map_state " << " " << movi_dbg1->motion_data.map_state << " " << pc_dbg2->motion_data.map_state << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->motion_data.time,pc_dbg2->motion_data.time))
//   {
//     out << " time " << " " << movi_dbg1->motion_data.time << " " << pc_dbg2->motion_data.time << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->motion_data.frameNo,pc_dbg2->motion_data.frameNo))
//   {
//     out << " frameNo " << " " << movi_dbg1->motion_data.frameNo << " " << pc_dbg2->motion_data.frameNo << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < 9 ; ++i)
//     if(diff_or_disp_float(movi_dbg1->motion_data.pose_rotation[i],pc_dbg2->motion_data.pose_rotation[i]))
//     {
//       out << "R " << " " << i << " " << movi_dbg1->motion_data.pose_rotation[i] << " " << pc_dbg2->motion_data.pose_rotation[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }

//   for(int i = 0 ; i < 3 ; ++i)
//     if(diff_or_disp_float(movi_dbg1->motion_data.pose_translation[i],pc_dbg2->motion_data.pose_translation[i]))
//     {
//       out << "T " << " " << i << " " << movi_dbg1->motion_data.pose_translation[i] << " " << pc_dbg2->motion_data.pose_translation[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }

//   for(int i = 0 ; i < 3 ; ++i)
//     if(diff_or_disp_float(movi_dbg1->motion_data.pose_angle[i],pc_dbg2->motion_data.pose_angle[i]))
//     {
//       out << "Degree " << " " << i << " " << movi_dbg1->motion_data.pose_angle[i] << " " << pc_dbg2->motion_data.pose_angle[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }


//   // for(int i = 0 ; i < 3 ; ++i)
//   // {
//   //   if(diff_or_disp_float(movi_dbg1->imu_data.accelData[i],pc_dbg2->imu_data.accelData[i]))
//   //   {
//   //     out << "IMU Info %d %.2f %.2f \n",i,movi_dbg1->imu_data.accelData[i],pc_dbg2->imu_data.accelData[i] << std::endl;
//   //     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//   //       break;
//   //     _abort_if_diff();
//   //   }
//   // }


//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_features_left,pc_dbg2->metrics_data.nb_features_left))
//   {
//     out << " nb_features_left " << " " << movi_dbg1->metrics_data.nb_features_left << " " << pc_dbg2->metrics_data.nb_features_left << std::endl;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_features_right,pc_dbg2->metrics_data.nb_features_right))
//   {
//     out << " nb_features_right " << " " << movi_dbg1->metrics_data.nb_features_right << " " << pc_dbg2->metrics_data.nb_features_right << std::endl;
//     _abort_if_diff();
//   }


//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc1_left,pc_dbg2->metrics_data.nb_matches_loc1_left))
//   {
//     out << " nb_matches_loc1_left " << " " << movi_dbg1->metrics_data.nb_matches_loc1_left << " " << pc_dbg2->metrics_data.nb_matches_loc1_left << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc1_right,pc_dbg2->metrics_data.nb_matches_loc1_right))
//   {
//     out << " nb_matches_loc1_right " << " " << movi_dbg1->metrics_data.nb_matches_loc1_right << " " << pc_dbg2->metrics_data.nb_matches_loc1_right << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc2_left,pc_dbg2->metrics_data.nb_matches_loc2_left))
//   {
//     out << " nb_matches_loc2_left " << " " << movi_dbg1->metrics_data.nb_matches_loc2_left << " " << pc_dbg2->metrics_data.nb_matches_loc2_left << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc2_right,pc_dbg2->metrics_data.nb_matches_loc2_right))
//   {
//     out << " nb_matches_loc2_right " << " " << movi_dbg1->metrics_data.nb_matches_loc2_right << " " << pc_dbg2->metrics_data.nb_matches_loc2_right << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_inliers,pc_dbg2->metrics_data.nb_inliers))
//   {
//     _abort_if_diff();
//     out << " nb_inliers " << " " << movi_dbg1->metrics_data.nb_inliers << " " << pc_dbg2->metrics_data.nb_inliers << std::endl;
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_local_map_size,pc_dbg2->metrics_data.nb_local_map_size))
//   {
//     out << " nb_local_map_size " << " " << movi_dbg1->metrics_data.nb_local_map_size << " " << pc_dbg2->metrics_data.nb_local_map_size << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_map_keyframe,pc_dbg2->metrics_data.nb_map_keyframe))
//   {
//     out << " nb_map_keyframe " << " " << movi_dbg1->metrics_data.nb_map_keyframe << " " << pc_dbg2->metrics_data.nb_map_keyframe << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->metrics_data.nb_map_features,pc_dbg2->metrics_data.nb_map_features))
//   {
//     out << " nb_map_features " << " " << movi_dbg1->metrics_data.nb_map_features << " " << pc_dbg2->metrics_data.nb_map_features << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->metrics_data.nb_map_3D,pc_dbg2->metrics_data.nb_map_3D))
//   {
//     out << " nb_map_3D " << " " << movi_dbg1->metrics_data.nb_map_3D << " " << pc_dbg2->metrics_data.nb_map_3D << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < 9+3+7 ; ++i)
//   {
//     float* f1 = (float*)&movi_dbg1->stereo;
//     float* f2 = (float*)&pc_dbg2->stereo;
//     if(diff_or_disp_float(f1[i],f2[i]))
//     {
//       out << " stereo["<<i<< "]" << f1[i] << " " << f2[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }


//   if(
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[0].begin,pc_dbg2->coordinates_range2.data[0].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[0].end,pc_dbg2->coordinates_range2.data[0].end)
//       ||
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[1].begin,pc_dbg2->coordinates_range2.data[1].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[1].end,pc_dbg2->coordinates_range2.data[1].end)
//     )
//   {
//     out << " coordinates_range2 1: " 
//       << movi_dbg1->coordinates_range2.data[0].begin << " " << movi_dbg1->coordinates_range2.data[0].end << " " << 
//       movi_dbg1->coordinates_range2.data[1].begin << " " << movi_dbg1->coordinates_range2.data[1].end << std::endl;
//     out << " coordinates_range2 2: "
//       << pc_dbg2->coordinates_range2.data[0].begin << " " << pc_dbg2->coordinates_range2.data[0].end << " " << 
//       pc_dbg2->coordinates_range2.data[1].begin << " " << pc_dbg2->coordinates_range2.data[1].end << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = movi_dbg1->coordinates_range2.data[0].begin ; i < movi_dbg1->coordinates_range2.data[1].end ; ++i)
//   {
//     if(
//         diff_or_disp_u16(movi_dbg1->coordinates[i].x,pc_dbg2->coordinates[i].x)
//         ||
//         diff_or_disp_u16(movi_dbg1->coordinates[i].y,pc_dbg2->coordinates[i].y)
//       )
//     {
//         out
//           << "Coordinate[" << i
//           << "]: x1:" << movi_dbg1->coordinates[i].x 
//           << " y1:" << movi_dbg1->coordinates[i].y
//           << " x2:" << pc_dbg2->coordinates[i].x
//           << " y2:" << pc_dbg2->coordinates[i].y << std::endl;
//         if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//           break;

//       _abort_if_diff();
//     }

//     for(int k = 0 ; k < 16 ; ++k)
//     {
//       if(diff_or_disp_short(movi_dbg1->descriptors[i].data[k],pc_dbg2->descriptors[i].data[k]))
//       {
//         out << "Descriptor[" << i << "][" << k << "]:" << movi_dbg1->descriptors[i].data[k] << " " << pc_dbg2->descriptors[i].data[k] << std::endl;
//         if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//           break;
//         _abort_if_diff();
//       }

//     }
//   }


//   for(int i = movi_dbg1->coordinates_range2.data[0].begin ; i < movi_dbg1->coordinates_range2.data[1].end ; ++i)
//   {
//     bool ok = false;
//     for(int j = pc_dbg2->coordinates_range2.data[0].begin ; j < pc_dbg2->coordinates_range2.data[1].end ; ++j)
//     {
//       if(
//           (movi_dbg1->coordinates[i].x==pc_dbg2->coordinates[j].x)
//           &&
//           (movi_dbg1->coordinates[i].y==pc_dbg2->coordinates[j].y)
//         )
//       {
//         ok = true;
//       }
//     }
//     if(!ok)
//     {
//       out << "!!! Features mismatches !!!\n" << std::endl;
//       goto break_feature_comparison;
//     }
//   }
//   break_feature_comparison:


//   if(diff_or_disp_short(movi_dbg1->nb_candidate1,pc_dbg2->nb_candidate1))
//   {
//     out << " nb_candidate1 " << " " << movi_dbg1->nb_candidate1 << " " << pc_dbg2->nb_candidate1 << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->match1_left.size,pc_dbg2->match1_left.size))
//   {
//     out << " match1_left.size " << " " << movi_dbg1->match1_left.size << " " << pc_dbg2->match1_left.size << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < movi_dbg1->match1_left.size ; ++i)
//   {
//     if (
//         diff_or_disp_u16(movi_dbg1->match1_left.pData[i].first,pc_dbg2->match1_left.pData[i].first)
//         ||
//         diff_or_disp_u16(movi_dbg1->match1_left.pData[i].second,pc_dbg2->match1_left.pData[i].second)
//       )
//     {
//       out << "match1_left first[ " << i << "]:"
//       << " first: " << movi_dbg1->match1_left.pData[i].first
//       << " " << pc_dbg2->match1_left.pData[i].first
//       << " second: " << movi_dbg1->match1_left.pData[i].second
//       << " " << pc_dbg2->match1_left.pData[i].second
//       << std::endl;

//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }


//   if(diff_or_disp_int(movi_dbg1->match1_right.size,pc_dbg2->match1_right.size))
//   {
//     out << " match1_right.size " << " " << movi_dbg1->match1_right.size << " " << pc_dbg2->match1_right.size << std::endl;
//     _abort_if_diff();
//   }


//   for(int i = 0 ; i < movi_dbg1->match1_right.size ; ++i)
//   {
//     if (
//         diff_or_disp_u16(movi_dbg1->match1_right.pData[i].first,pc_dbg2->match1_right.pData[i].first)
//         ||
//         diff_or_disp_u16(movi_dbg1->match1_right.pData[i].second,pc_dbg2->match1_right.pData[i].second)
//       )
//     {
//       out << "match1_right first[" << i << "]: first:"
//       << movi_dbg1->match1_right.pData[i].first << " " << pc_dbg2->match1_right.pData[i].first
//       << " second "
//       << movi_dbg1->match1_right.pData[i].second << " " << pc_dbg2->match1_right.pData[i].second
//       << std::endl;

//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output1.C,pc_dbg2->loc_output1.C))
//   {
//     out << "loc_output1.C " << " " << movi_dbg1->loc_output1.C << " " << pc_dbg2->loc_output1.C << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   for(int k = 0 ; k < 6 ; ++k)
//   {
//     if(diff_or_disp_float(movi_dbg1->loc_output1.C,pc_dbg2->loc_output1.C))
//     {
//       out << "loc_output1.delta[" << k << "] " << movi_dbg1->loc_output1.C << " "<< pc_dbg2->loc_output1.C << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//       _abort_if_diff();
//     }
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output1.lambda,pc_dbg2->loc_output1.lambda))
//   {
//     out << "loc_output1.lambda " << " " << movi_dbg1->loc_output1.lambda << " " << pc_dbg2->loc_output1.lambda << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output1.error_before_opt,pc_dbg2->loc_output1.error_before_opt))
//   {
//     out << "loc_output1.error_before_opt " << " " << movi_dbg1->loc_output1.error_before_opt << " " << pc_dbg2->loc_output1.error_before_opt << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_float(movi_dbg1->loc_output1.curr,pc_dbg2->loc_output1.curr))
//   {
//     out << "loc_output1.curr " << " " << movi_dbg1->loc_output1.curr << " " << pc_dbg2->loc_output1.curr << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output1.rho,pc_dbg2->loc_output1.rho))
//   {
//     out << "loc_output1.rho " << " " << movi_dbg1->loc_output1.rho << " " << pc_dbg2->loc_output1.rho << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output1.alpha,pc_dbg2->loc_output1.alpha))
//   {
//     out << "loc_output1.alpha " << " " << movi_dbg1->loc_output1.alpha << " " << pc_dbg2->loc_output1.alpha << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->loc_output1.printAlpha,pc_dbg2->loc_output1.printAlpha))
//   {
//     out << "loc_output1.printAlpha " << " " << movi_dbg1->loc_output1.printAlpha << " " << pc_dbg2->loc_output1.printAlpha << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output1;
//     _abort_if_diff();
//   }

//   end_of_loc_output1:

//   if(diff_or_disp_short(movi_dbg1->nb_candidate2,pc_dbg2->nb_candidate2))
//   {
//     out << " nb_candidate2 " << " " << movi_dbg1->nb_candidate2 << " " << pc_dbg2->nb_candidate2 << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->match2_left.size,pc_dbg2->match2_left.size))
//   {
//     out << " match2_left.size "  << " " << movi_dbg1->match2_left.size << " " << pc_dbg2->match2_left.size << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < movi_dbg1->match2_left.size ; ++i)
//   {
//     if (
//         diff_or_disp_u16(movi_dbg1->match2_left.pData[i].first,pc_dbg2->match2_left.pData[i].first)
//         ||
//         diff_or_disp_u16(movi_dbg1->match2_left.pData[i].second,pc_dbg2->match2_left.pData[i].second)
//       )
//     {
//       out << "match2_left first[" << i
//         << "]: first :"
//         << movi_dbg1->match2_left.pData[i].first << " " << pc_dbg2->match2_left.pData[i].first
//         << " second " <<  movi_dbg1->match2_left.pData[i].second << " " << pc_dbg2->match2_left.pData[i].second
//         << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }


//   if(diff_or_disp_int(movi_dbg1->match2_right.size,pc_dbg2->match2_right.size))
//   {
//     out << " match2_right.size " << " " << movi_dbg1->match2_right.size << " " << pc_dbg2->match2_right.size << std::endl;
//     _abort_if_diff();
//   }


//   for(int i = 0 ; i < movi_dbg1->match2_right.size ; ++i)
//   {
//     if (
//         diff_or_disp_u16(movi_dbg1->match2_right.pData[i].first,pc_dbg2->match2_right.pData[i].first)
//         ||
//         diff_or_disp_u16(movi_dbg1->match2_right.pData[i].second,pc_dbg2->match2_right.pData[i].second)
//       )
//     {
//       out << "match2_right first[" << i << "]: first: " <<
//         movi_dbg1->match2_right.pData[i].first << " " << pc_dbg2->match2_right.pData[i].first << " second " << 
//         movi_dbg1->match2_right.pData[i].second << " " << pc_dbg2->match2_right.pData[i].second
//         << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output2.C,pc_dbg2->loc_output2.C))
//   {
//     out << "loc_output2.C " << " " << movi_dbg1->loc_output2.C << " " << pc_dbg2->loc_output2.C << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//       goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   for(int k = 0 ; k < 6 ; ++k)
//   {
//     if(diff_or_disp_float(movi_dbg1->loc_output2.C,pc_dbg2->loc_output2.C))
//     {
//       out << "loc_output2.delta[" << k << "] " << movi_dbg1->loc_output2.C << " " << pc_dbg2->loc_output2.C << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//       _abort_if_diff();
//     }
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output2.lambda,pc_dbg2->loc_output2.lambda))
//   {
//     out << "loc_output2.lambda " << " " << movi_dbg1->loc_output2.lambda << " " << pc_dbg2->loc_output2.lambda << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output2.error_before_opt,pc_dbg2->loc_output2.error_before_opt))
//   {
//     out << "loc_output2.error_before_opt " << " " << movi_dbg1->loc_output2.error_before_opt << " " << pc_dbg2->loc_output2.error_before_opt << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_float(movi_dbg1->loc_output2.curr,pc_dbg2->loc_output2.curr))
//   {
//     out << "loc_output2.curr " << " " << movi_dbg1->loc_output2.curr << " " << pc_dbg2->loc_output2.curr << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output2.rho,pc_dbg2->loc_output2.rho))
//   {
//     out << "loc_output2.rho " << " " << movi_dbg1->loc_output2.rho << " " << pc_dbg2->loc_output2.rho << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->loc_output2.alpha,pc_dbg2->loc_output2.alpha))
//   {
//     out << "loc_output2.alpha " << " " << movi_dbg1->loc_output2.alpha << " " << pc_dbg2->loc_output2.alpha << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->loc_output2.printAlpha,pc_dbg2->loc_output2.printAlpha))
//   {
//     out << "loc_output2.printAlpha " << " " << movi_dbg1->loc_output2.printAlpha << " " << pc_dbg2->loc_output2.printAlpha << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         goto end_of_loc_output2;
//     _abort_if_diff();
//   }

//   end_of_loc_output2:

//   int nb_inlier_print = 0;

//   if(
//       diff_or_disp_int(movi_dbg1->inliers_3d_range2.data[0].begin,pc_dbg2->inliers_3d_range2.data[0].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->inliers_3d_range2.data[0].end,pc_dbg2->inliers_3d_range2.data[0].end)
//       ||
//       diff_or_disp_int(movi_dbg1->inliers_3d_range2.data[1].begin,pc_dbg2->inliers_3d_range2.data[1].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->inliers_3d_range2.data[1].end,pc_dbg2->inliers_3d_range2.data[1].end)
//     )
//   {
//     out << " inliers_3d_range2 1: " <<
//       movi_dbg1->inliers_3d_range2.data[0].begin << " " << movi_dbg1->inliers_3d_range2.data[0].end << " " << 
//       movi_dbg1->inliers_3d_range2.data[1].begin << " " << movi_dbg1->inliers_3d_range2.data[1].end << std::endl;
//     out << " inliers_3d_range2 2: " <<
//       pc_dbg2->inliers_3d_range2.data[0].begin << " " << pc_dbg2->inliers_3d_range2.data[0].end << " " << 
//       pc_dbg2->inliers_3d_range2.data[1].begin << " " << pc_dbg2->inliers_3d_range2.data[1].end << std::endl;
//     _abort_if_diff();
//   }
//   else {
//      nb_inlier_print = pc_dbg2->inliers_3d_range2.data[1].end;
//   }

//   for(int i = 0; i < nb_inlier_print ; ++i)
//   {
//     if(
//         diff_or_disp_float(movi_dbg1->inliers_3d[i].data[0],pc_dbg2->inliers_3d[i].data[0])
//         ||
//         diff_or_disp_float(movi_dbg1->inliers_3d[i].data[1],pc_dbg2->inliers_3d[i].data[1])
//         ||
//         diff_or_disp_float(movi_dbg1->inliers_3d[i].data[2],pc_dbg2->inliers_3d[i].data[2])
//       )
//     {
//       out << "XVec3[" << i
//         << "]: x1:"
//         << movi_dbg1->inliers_3d[i].data[0]
//         << " x2:" << 
//         pc_dbg2->inliers_3d[i].data[0]
//         << " y1:" <<
//         movi_dbg1->inliers_3d[i].data[1]
//         << " y2:" <<
//         pc_dbg2->inliers_3d[i].data[1]
//         << " z1:" <<
//         movi_dbg1->inliers_3d[i].data[2]
//         << " z2:" <<
//         pc_dbg2->inliers_3d[i].data[2]
//         << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }

//   if(diff_or_disp_short(movi_dbg1->debug_map.nb_added_feature,pc_dbg2->debug_map.nb_added_feature))
//   {
//     out << "debug map nb_added_feature " << " " << movi_dbg1->debug_map.nb_added_feature << " " << pc_dbg2->debug_map.nb_added_feature << std::endl;
//     _abort_if_diff();
//   }


//   for(int i = 0 ; i < 9 ; ++i)
//   if(diff_or_disp_float(movi_dbg1->debug_map.added_pose.rotation.data[i],pc_dbg2->debug_map.added_pose.rotation.data[i]))
//   {
//     out << "debug map added_pose "<< " " << i<< " " << movi_dbg1->debug_map.added_pose.rotation.data[i]<< " " << pc_dbg2->debug_map.added_pose.rotation.data[i] << std::endl;
//     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//     _abort_if_diff();
//   }


//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_1.radius_x,pc_dbg2->debug_map.match_config_1.radius_x))
//   {
//     out << "debug map match_config_1.radius_x "<<  movi_dbg1->debug_map.match_config_1.radius_x<< " " << pc_dbg2->debug_map.match_config_1.radius_x << std::endl;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_1.radius_y,pc_dbg2->debug_map.match_config_1.radius_y))
//   {
//     out << "debug map match_config_1.radius_y "<<  movi_dbg1->debug_map.match_config_1.radius_y<< " " << pc_dbg2->debug_map.match_config_1.radius_y << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->debug_map.match_config_1.score_min,pc_dbg2->debug_map.match_config_1.score_min))
//   {
//     out << "debug map match_config_1.score_min "<<  movi_dbg1->debug_map.match_config_1.score_min<< " " << pc_dbg2->debug_map.match_config_1.score_min << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_enable1_with_previous[i],pc_dbg2->debug_map.nb_enable1_with_previous[i]))
//     {
//       out << "debug map nb_enable1_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_enable1_with_previous[i] << " " << pc_dbg2->debug_map.nb_enable1_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_projected_with_previous[i],pc_dbg2->debug_map.nb_projected_with_previous[i]))
//     {
//       out << "debug map nb_projected_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_projected_with_previous[i] << " " << pc_dbg2->debug_map.nb_projected_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_match1_with_previous[i],pc_dbg2->debug_map.nb_match1_with_previous[i]))
//     {
//       out << "debug map nb_match1_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_match1_with_previous[i] << " " << pc_dbg2->debug_map.nb_match1_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_tracked_with_previous[i],pc_dbg2->debug_map.nb_tracked_with_previous[i]))
//     {
//       out << "debug map nb_tracked_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_tracked_with_previous[i] << " " << pc_dbg2->debug_map.nb_tracked_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }


//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_2.radius_x,pc_dbg2->debug_map.match_config_2.radius_x))
//   {
//     out << "debug map match_config_2.radius_x "<<  movi_dbg1->debug_map.match_config_2.radius_x<< " " << pc_dbg2->debug_map.match_config_2.radius_x << std::endl;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_2.radius_y,pc_dbg2->debug_map.match_config_2.radius_y))
//   {
//     out << "debug map match_config_2.radius_y "<<  movi_dbg1->debug_map.match_config_2.radius_y<< " " << pc_dbg2->debug_map.match_config_2.radius_y << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->debug_map.match_config_2.score_min,pc_dbg2->debug_map.match_config_2.score_min))
//   {
//     out << "debug map match_config_2.score_min "<<  movi_dbg1->debug_map.match_config_2.score_min<< " " << pc_dbg2->debug_map.match_config_2.score_min << std::endl;
//     _abort_if_diff();
//   }

  

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_enable2_with_previous[i],pc_dbg2->debug_map.nb_enable2_with_previous[i]))
//     {
//       out << "debug map nb_enable2_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_enable2_with_previous[i] << " " << pc_dbg2->debug_map.nb_enable2_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }


//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_match2_with_previous[i],pc_dbg2->debug_map.nb_match2_with_previous[i]))
//     {
//       out << "debug map nb_match2_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_match2_with_previous[i] << " " << pc_dbg2->debug_map.nb_match2_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }

//   for(int i = 0 ; i < 2 ; ++i)
//   {
//     if(diff_or_disp_int(movi_dbg1->debug_map.nb_new3d_with_previous[i],pc_dbg2->debug_map.nb_new3d_with_previous[i]))
//     {
//       out << "debug map nb_new3d_with_previous["<< i << "] " <<  movi_dbg1->debug_map.nb_new3d_with_previous[i] << " " << pc_dbg2->debug_map.nb_new3d_with_previous[i] << std::endl;
//       _abort_if_diff();
//     }
//   }


//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_3.radius_x,pc_dbg2->debug_map.match_config_3.radius_x))
//   {
//     out << "debug map match_config_3.radius_x "<<  movi_dbg1->debug_map.match_config_3.radius_x<< " " << pc_dbg2->debug_map.match_config_3.radius_x << std::endl;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_int(movi_dbg1->debug_map.match_config_3.radius_y,pc_dbg2->debug_map.match_config_3.radius_y))
//   {
//     out << "debug map match_config_3.radius_y "<<  movi_dbg1->debug_map.match_config_3.radius_y<< " " << pc_dbg2->debug_map.match_config_3.radius_y << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->debug_map.match_config_3.score_min,pc_dbg2->debug_map.match_config_3.score_min))
//   {
//     out << "debug map match_config_3.score_min "<<  movi_dbg1->debug_map.match_config_3.score_min<< " " << pc_dbg2->debug_map.match_config_3.score_min << std::endl;
//     _abort_if_diff();
//   }


//   if(diff_or_disp_int(movi_dbg1->debug_map.nb_match_with_stereo_matching,pc_dbg2->debug_map.nb_match_with_stereo_matching))
//   {
//     out << "debug map nb_match_with_stereo_matching "<<  movi_dbg1->debug_map.nb_match_with_stereo_matching << " " << pc_dbg2->debug_map.nb_match_with_stereo_matching << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->debug_map.nb_new3d_with_stereo,pc_dbg2->debug_map.nb_new3d_with_stereo))
//   {
//     out << "debug map nb_new3d_with_stereo "<<  movi_dbg1->debug_map.nb_new3d_with_stereo << " " << pc_dbg2->debug_map.nb_new3d_with_stereo << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->debug_map.nb_observation,pc_dbg2->debug_map.nb_observation))
//   {
//     out << "debug map nb_observation "<<  movi_dbg1->debug_map.nb_observation << " " << pc_dbg2->debug_map.nb_observation << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->debug_map.nb_opt_pose,pc_dbg2->debug_map.nb_opt_pose))
//   {
//     out << "debug map nb_opt_pose "<<  movi_dbg1->debug_map.nb_opt_pose << " " << pc_dbg2->debug_map.nb_opt_pose << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->debug_map.n_iter,pc_dbg2->debug_map.n_iter))
//   {
//     out << "debug map n_iter "<<  movi_dbg1->debug_map.n_iter << " " << pc_dbg2->debug_map.n_iter << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->debug_map.error,pc_dbg2->debug_map.error))
//   {
//     out << "debug map error "<<  movi_dbg1->debug_map.error << " " << pc_dbg2->debug_map.error << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->debug_map.cr,pc_dbg2->debug_map.cr))
//   {
//     out << "debug map cr "<<  movi_dbg1->debug_map.cr << " " << pc_dbg2->debug_map.cr << std::endl;
//     _abort_if_diff();
//   }

//   if(
//       diff_or_disp_int(movi_dbg1->debug_map.local_map_range.data[0].begin,pc_dbg2->debug_map.local_map_range.data[0].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->debug_map.local_map_range.data[0].end,pc_dbg2->debug_map.local_map_range.data[0].end)
//       ||
//       diff_or_disp_int(movi_dbg1->debug_map.local_map_range.data[1].begin,pc_dbg2->debug_map.local_map_range.data[1].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->debug_map.local_map_range.data[1].end,pc_dbg2->debug_map.local_map_range.data[1].end)
//     )
//   {
//     out << " inliers_3d_range2 1: " <<
//       movi_dbg1->debug_map.local_map_range.data[0].begin << " " << movi_dbg1->debug_map.local_map_range.data[0].end << " " << 
//       movi_dbg1->debug_map.local_map_range.data[1].begin << " " << movi_dbg1->debug_map.local_map_range.data[1].end << std::endl;
//     out << " inliers_3d_range2 2: " <<
//       pc_dbg2->debug_map.local_map_range.data[0].begin << " " << pc_dbg2->debug_map.local_map_range.data[0].end << " " << 
//       pc_dbg2->debug_map.local_map_range.data[1].begin << " " << pc_dbg2->debug_map.local_map_range.data[1].end << std::endl;
//     _abort_if_diff();
//   }


//   if(
//       diff_or_disp_float(movi_dbg1->debug_map.first_3d.data[0],pc_dbg2->debug_map.first_3d.data[0])
//       ||
//       diff_or_disp_float(movi_dbg1->debug_map.first_3d.data[1],pc_dbg2->debug_map.first_3d.data[1])
//       ||
//       diff_or_disp_float(movi_dbg1->debug_map.first_3d.data[2],pc_dbg2->debug_map.first_3d.data[2])
//     )
//   {
//     out << "First p3d of the map "
//       << ": x1:"
//       << movi_dbg1->debug_map.first_3d.data[0]
//       << " x2:" << 
//       pc_dbg2->debug_map.first_3d.data[0]
//       << " y1:" <<
//       movi_dbg1->debug_map.first_3d.data[1]
//       << " y2:" <<
//       pc_dbg2->debug_map.first_3d.data[1]
//       << " z1:" <<
//       movi_dbg1->debug_map.first_3d.data[2]
//       << " z2:" <<
//       pc_dbg2->debug_map.first_3d.data[2]
//       << std::endl;
//     _abort_if_diff();
//   }


//   if(
//       diff_or_disp_float(movi_dbg1->debug_map.last_3d.data[0],pc_dbg2->debug_map.last_3d.data[0])
//       ||
//       diff_or_disp_float(movi_dbg1->debug_map.last_3d.data[1],pc_dbg2->debug_map.last_3d.data[1])
//       ||
//       diff_or_disp_float(movi_dbg1->debug_map.last_3d.data[2],pc_dbg2->debug_map.last_3d.data[2])
//     )
//   {
//     out << "First p3d of the map "
//       << ": x1:"
//       << movi_dbg1->debug_map.last_3d.data[0]
//       << " x2:" << 
//       pc_dbg2->debug_map.last_3d.data[0]
//       << " y1:" <<
//       movi_dbg1->debug_map.last_3d.data[1]
//       << " y2:" <<
//       pc_dbg2->debug_map.last_3d.data[1]
//       << " z1:" <<
//       movi_dbg1->debug_map.last_3d.data[2]
//       << " z2:" <<
//       pc_dbg2->debug_map.last_3d.data[2]
//       << std::endl;
//     _abort_if_diff();
//   }


//   float pc_size_of_tmp_buff = pc_dbg2->tmpBuff[0];
//   float movi_size_of_tmp_buff = movi_dbg1->tmpBuff[0];
//   if(pc_size_of_tmp_buff!=movi_size_of_tmp_buff)
//   {
//     out << " movi_size_of_tmp_buff!=pc_size_of_tmp_buff " << movi_size_of_tmp_buff << " != " << pc_size_of_tmp_buff << std::endl;
//     int size = std::min(pc_size_of_tmp_buff,movi_size_of_tmp_buff);
//     for(int i = 0 ; i < size ; ++i)
//     {
//       if(pc_dbg2->tmpBuff[i]!=movi_dbg1->tmpBuff[i])
//       {
//         out << "movi tmpBuff[" << i <<"] != pc tmpBuff[" << i << "]: " << movi_dbg1->tmpBuff[i] << " != " << pc_dbg2->tmpBuff[i] << std::endl;
//       }
//     }
//   }

 

// }



// void check_debug_struct(MixedModeStruct* movi_dbg1, MixedModeStruct* pc_dbg2, int verbose_level, std::ostream& out)
// {
//   if (verbose_level==CHECK_VERBOSE_OFF) return;


//   if(diff_or_disp_short(movi_dbg1->motion_data.device_state,pc_dbg2->motion_data.device_state))
//   {
//     out << " device_state " << " " << movi_dbg1->motion_data.device_state << " " << pc_dbg2->motion_data.device_state << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->motion_data.map_state,pc_dbg2->motion_data.map_state))
//   {
//     out << " map_state " << " " << movi_dbg1->motion_data.map_state << " " << pc_dbg2->motion_data.map_state << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_float(movi_dbg1->motion_data.time,pc_dbg2->motion_data.time))
//   {
//     out << " time " << " " << movi_dbg1->motion_data.time << " " << pc_dbg2->motion_data.time << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->motion_data.frameNo,pc_dbg2->motion_data.frameNo))
//   {
//     out << " frameNo " << " " << movi_dbg1->motion_data.frameNo << " " << pc_dbg2->motion_data.frameNo << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < 9 ; ++i)
//     if(diff_or_disp_float(movi_dbg1->motion_data.pose_rotation[i],pc_dbg2->motion_data.pose_rotation[i]))
//     {
//       out << "R " << " " << i << " " << movi_dbg1->motion_data.pose_rotation[i] << " " << pc_dbg2->motion_data.pose_rotation[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }

//   for(int i = 0 ; i < 3 ; ++i)
//     if(diff_or_disp_float(movi_dbg1->motion_data.pose_translation[i],pc_dbg2->motion_data.pose_translation[i]))
//     {
//       out << "T " << " " << i << " " << movi_dbg1->motion_data.pose_translation[i] << " " << pc_dbg2->motion_data.pose_translation[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }

//   for(int i = 0 ; i < 3 ; ++i)
//     if(diff_or_disp_float(movi_dbg1->motion_data.pose_angle[i],pc_dbg2->motion_data.pose_angle[i]))
//     {
//       out << "Degree " << " " << i << " " << movi_dbg1->motion_data.pose_angle[i] << " " << pc_dbg2->motion_data.pose_angle[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }


//   // for(int i = 0 ; i < 3 ; ++i)
//   // {
//   //   if(diff_or_disp_float(movi_dbg1->imu_data.accelData[i],pc_dbg2->imu_data.accelData[i]))
//   //   {
//   //     out << "IMU Info %d %.2f %.2f \n",i,movi_dbg1->imu_data.accelData[i],pc_dbg2->imu_data.accelData[i] << std::endl;
//   //     if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//   //       break;
//   //     _abort_if_diff();
//   //   }
//   // }


//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_features_left,pc_dbg2->metrics_data.nb_features_left))
//   {
//     out << " nb_features_left " << " " << movi_dbg1->metrics_data.nb_features_left << " " << pc_dbg2->metrics_data.nb_features_left << std::endl;
//     _abort_if_diff();
//   }
  
//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_features_right,pc_dbg2->metrics_data.nb_features_right))
//   {
//     out << " nb_features_right " << " " << movi_dbg1->metrics_data.nb_features_right << " " << pc_dbg2->metrics_data.nb_features_right << std::endl;
//     _abort_if_diff();
//   }


//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc1_left,pc_dbg2->metrics_data.nb_matches_loc1_left))
//   {
//     out << " nb_matches_loc1_left " << " " << movi_dbg1->metrics_data.nb_matches_loc1_left << " " << pc_dbg2->metrics_data.nb_matches_loc1_left << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc1_right,pc_dbg2->metrics_data.nb_matches_loc1_right))
//   {
//     out << " nb_matches_loc1_right " << " " << movi_dbg1->metrics_data.nb_matches_loc1_right << " " << pc_dbg2->metrics_data.nb_matches_loc1_right << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc2_left,pc_dbg2->metrics_data.nb_matches_loc2_left))
//   {
//     out << " nb_matches_loc2_left " << " " << movi_dbg1->metrics_data.nb_matches_loc2_left << " " << pc_dbg2->metrics_data.nb_matches_loc2_left << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_matches_loc2_right,pc_dbg2->metrics_data.nb_matches_loc2_right))
//   {
//     out << " nb_matches_loc2_right " << " " << movi_dbg1->metrics_data.nb_matches_loc2_right << " " << pc_dbg2->metrics_data.nb_matches_loc2_right << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_inliers,pc_dbg2->metrics_data.nb_inliers))
//   {
//     _abort_if_diff();
//     out << " nb_inliers " << " " << movi_dbg1->metrics_data.nb_inliers << " " << pc_dbg2->metrics_data.nb_inliers << std::endl;
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_local_map_size,pc_dbg2->metrics_data.nb_local_map_size))
//   {
//     out << " nb_local_map_size " << " " << movi_dbg1->metrics_data.nb_local_map_size << " " << pc_dbg2->metrics_data.nb_local_map_size << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_short(movi_dbg1->metrics_data.nb_map_keyframe,pc_dbg2->metrics_data.nb_map_keyframe))
//   {
//     out << " nb_map_keyframe " << " " << movi_dbg1->metrics_data.nb_map_keyframe << " " << pc_dbg2->metrics_data.nb_map_keyframe << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->metrics_data.nb_map_features,pc_dbg2->metrics_data.nb_map_features))
//   {
//     out << " nb_map_features " << " " << movi_dbg1->metrics_data.nb_map_features << " " << pc_dbg2->metrics_data.nb_map_features << std::endl;
//     _abort_if_diff();
//   }

//   if(diff_or_disp_int(movi_dbg1->metrics_data.nb_map_3D,pc_dbg2->metrics_data.nb_map_3D))
//   {
//     out << " nb_map_3D " << " " << movi_dbg1->metrics_data.nb_map_3D << " " << pc_dbg2->metrics_data.nb_map_3D << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = 0 ; i < 9+3+7 ; ++i)
//   {
//     float* f1 = (float*)&movi_dbg1->stereo;
//     float* f2 = (float*)&pc_dbg2->stereo;
//     if(diff_or_disp_float(f1[i],f2[i]))
//     {
//       out << " stereo["<<i<< "]" << f1[i] << " " << f2[i] << std::endl;
//       if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//         break;
//       _abort_if_diff();
//     }
//   }


//   if(
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[0].begin,pc_dbg2->coordinates_range2.data[0].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[0].end,pc_dbg2->coordinates_range2.data[0].end)
//       ||
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[1].begin,pc_dbg2->coordinates_range2.data[1].begin)
//       ||
//       diff_or_disp_int(movi_dbg1->coordinates_range2.data[1].end,pc_dbg2->coordinates_range2.data[1].end)
//     )
//   {
//     out << " coordinates_range2 1: " 
//       << movi_dbg1->coordinates_range2.data[0].begin << " " << movi_dbg1->coordinates_range2.data[0].end << " " << 
//       movi_dbg1->coordinates_range2.data[1].begin << " " << movi_dbg1->coordinates_range2.data[1].end << std::endl;
//     out << " coordinates_range2 2: "
//       << pc_dbg2->coordinates_range2.data[0].begin << " " << pc_dbg2->coordinates_range2.data[0].end << " " << 
//       pc_dbg2->coordinates_range2.data[1].begin << " " << pc_dbg2->coordinates_range2.data[1].end << std::endl;
//     _abort_if_diff();
//   }

//   for(int i = movi_dbg1->coordinates_range2.data[0].begin ; i < movi_dbg1->coordinates_range2.data[1].end ; ++i)
//   {
//     if(
//         diff_or_disp_u16(movi_dbg1->coordinates[i].x,pc_dbg2->coordinates[i].x)
//         ||
//         diff_or_disp_u16(movi_dbg1->coordinates[i].y,pc_dbg2->coordinates[i].y)
//       )
//     {
//         out
//           << "Coordinate[" << i
//           << "]: x1:" << movi_dbg1->coordinates[i].x 
//           << " y1:" << movi_dbg1->coordinates[i].y
//           << " x2:" << pc_dbg2->coordinates[i].x
//           << " y2:" << pc_dbg2->coordinates[i].y << std::endl;
//         if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//           break;

//       _abort_if_diff();
//     }

//     for(int k = 0 ; k < 16 ; ++k)
//     {
//       if(diff_or_disp_short(movi_dbg1->descriptors[i].data[k],pc_dbg2->descriptors[i].data[k]))
//       {
//         out << "Descriptor[" << i << "][" << k << "]:" << movi_dbg1->descriptors[i].data[k] << " " << pc_dbg2->descriptors[i].data[k] << std::endl;
//         if(verbose_level==CHECK_VERBOSE_ESSENTIAL)
//           break;
//         _abort_if_diff();
//       }

//     }
//   }


//   for(int i = movi_dbg1->coordinates_range2.data[0].begin ; i < movi_dbg1->coordinates_range2.data[1].end ; ++i)
//   {
//     bool ok = false;
//     for(int j = pc_dbg2->coordinates_range2.data[0].begin ; j < pc_dbg2->coordinates_range2.data[1].end ; ++j)
//     {
//       if(
//           (movi_dbg1->coordinates[i].x==pc_dbg2->coordinates[j].x)
//           &&
//           (movi_dbg1->coordinates[i].y==pc_dbg2->coordinates[j].y)
//         )
//       {
//         ok = true;
//       }
//     }
//     if(!ok)
//     {
//       out << "!!! Features mismatches !!!\n" << std::endl;
//       goto break_feature_comparison_1;
//     }
//   }
//   break_feature_comparison_1:
//   return;

// }


// void check_debug_buffer(char* movi_buf1, char* pc_buf2, int verbose_level)
// {
//   DebugStruct movi_dbg1,pc_dbg2;

//   load_dbg(&movi_dbg1, movi_buf1);
//   load_dbg(&pc_dbg2, pc_buf2);

//   check_debug_struct(&movi_dbg1,&pc_dbg2,verbose_level);
// }


// void check_debug_file(std::string folder_slam, std::string filename, std::string title, int verbose_level)
// {
//   char * buf1 = (char*) aligned_malloc(DebugStructSize);
//   char * buf2 = (char*) aligned_malloc(DebugStructSize);
//   memset(buf1,0,DebugStructSize);
//   memset(buf2,0,DebugStructSize);

//   printf(" Check binary reference %s with current output %s\n",(folder_slam+filename).c_str(),filename.c_str());
//   std::ifstream(folder_slam+filename).read(buf1,DebugStructSize);
//   std::ifstream(filename).read(buf2,DebugStructSize);

//   check_debug_buffer(buf1,buf2,verbose_level);

//   aligned_free(buf1);
//   aligned_free(buf2);
// }

// void disp_dbg_main_info(DebugStruct* dbg)
// {
//   Metrics const& metrics = dbg->metrics_data;
//   std::cout << " Features      : " << metrics.nb_features_left << "," << metrics.nb_features_right << std::endl;
//   std::cout << " NB inliers    : " << metrics.nb_inliers << std::endl;
//   std::cout << " Local map     : " << metrics.nb_local_map_size << std::endl;
//   std::cout << " Map Keyrames  : " << metrics.nb_map_keyframe << std::endl;
// }

