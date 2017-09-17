#include <bits/stringfwd.h>
#include <string>

#include "Kmer_handler.h"
#include "log.h"

using namespace std;

/**
 * Constructor 
 * @param length : kmer length 
 */
Kmer_handler::Kmer_handler(uint8_t length, Contig_handler * c_handler)
{
    kmer_length = length;   
    ch = c_handler;
    //some requirement by google sparsehash to use erase
    kmer_counter.set_deleted_key(SPARSEHASH_DELETE_KEY);     
}

Kmer_handler::Kmer_handler(uint8_t length, char * shc_logname)
{
    kmer_length = length;
    this->shc_logname = shc_logname;   
    is_use_set = true;
    kmer_counter.set_deleted_key(SPARSEHASH_DELETE_KEY);       
}

Kmer_handler::Kmer_handler(uint8_t length, char * shc_logname, kmer_count_t min_count, 
                Contig_handler::size_type min_len, double R_threshold, 
                uint8_t rmer_len, bool is_use_set)
{
    kmer_length = length;
    this->shc_logname = shc_logname;
    this->min_count = min_count;
    this->min_len = min_len;
    this->r_thresh = R_threshold;
    this->rmer_len = rmer_len;
    this->is_use_set = is_use_set;
    kmer_counter.set_deleted_key(SPARSEHASH_DELETE_KEY);   
}

/**
 * Helper function to put numeric kmer into dictionary
 * @param kmer
 * @param count
 * @return 
 */
int Kmer_handler::add_kmer(uint64_t kmer, kmer_count_t count)
{
    Kmer_info kmer_info;
    kmer_info.count += count;
    kmer_counter[kmer] = kmer_info;
}

/**
 * write all kmer and count into file, separated by space
 * @param filename
 */
void Kmer_handler::dump_kmers_to_file(string * filename) {
    
    ofstream outfile (filename->c_str());
       
    Kmer_counter_map_iterator it;
    
    char kmer_base[KMER_HOLDER_LEN]; 
            
    for (it = kmer_counter.begin(); it != kmer_counter.end(); it++) {
        
        uint64_t kmer_val = it->first;
        kmer_count_t count = it->second.count;
        
        
        decode_byte_list((uint8_t*) &kmer_val, kmer_base, kmer_length); 
        kmer_base[kmer_length] = '\0';
        outfile << kmer_base << " " << count << endl;
        
    }
    
    outfile.close();
    
}

/**
 * The function reads "kmer count" pair from file, each line denotes a
 * pair, and the delimitor is a tab
 * @param filename
 * @return 
 */
int Kmer_handler::read_sequence_from_file (string filename) 
{
    shc_log_info(shc_logname, "Reading kmers from the file\n");
    ifstream fileReader (filename.c_str());   
    std::string line_s;
    std::string kmer_s;
    kmer_count_t count;  
    int count_temp;
    uint64_t kmer; 
    char back_string[33];
    back_string[kmer_length] = '\0';
    
    std::getline(fileReader, line_s);
    
    while (! fileReader.eof() ) 
    {     
        std::stringstream kmer_and_count(line_s);
        kmer_and_count >> kmer_s; 
        kmer_and_count >> count_temp;
        if (count_temp > MAX_COUNT)
        {
            return 1;
            shc_log_error("kmer_count_t unable to hold count\n");        
        }
        count = (kmer_count_t)count_temp;        
        
        struct Kmer_info kmer_info;

        encode_kmer(line_s.c_str(), &kmer, kmer_length);        
        
        kmer_info.count = count;
        kmer_counter[kmer] = kmer_info;        
        
        std::getline(fileReader, line_s);
    }  
    shc_log_info(shc_logname, "Finish reading kmers from the file\n");
    return 0;
}

// 1 is the first bit
bool Kmer_handler::is_info_ith_bit_set(uint8_t info, uint8_t i)
{
    if (i==1)
    {
        return (info & (uint8_t)(B1))>0;
    }
    else
    {
        return (info & (((uint8_t)(B1))<<(i-1)))>0;
    }
}

/**
 * This function checks the info in a kmer if it has prefix(1st) or suffix(2nd) 
 * bit relating to another kmer. 
 */
inline bool Kmer_handler::is_kmer_info_has_ps(uint8_t info)
{
    return info > 0;
}

/**
 *  This function checks the info in a kmer if it has prefix relating to another
 *  kmer. 
 */
inline bool Kmer_handler::is_kmer_info_has_prefix(uint8_t info)
{
    return ((uint8_t)B1234 & info) > 0;
}

/**
 *  This function checks the info in a kmer if it has suffix relating to another
 *  kmer. 
 */
inline bool Kmer_handler::is_kmer_info_has_suffix(uint8_t info)
{
    return ((uint8_t)B5678 & info) > 0;
}

/**
 * This function sorts kmer dictionary according to count information, and 
 * store the descending list in a vector.
 */
void Kmer_handler::sort_kmer_descending_count()
{
    unsigned long start, end;
    unsigned long num_kmers = kmer_counter.size();   
    Kmer_counter_map_iterator it;
    
    shc_log_info(shc_logname, 
            "Sorting %ld %umer based on counts\n", num_kmers, kmer_length);
    
    unsigned long count = 0;

    for (it = kmer_counter.begin(); it != kmer_counter.end(); it++) {

        if (it->second.count > 0) {

            Kmer_Occurence_Pair kp (it->first, it->second.count);            
            kmer_descend_list.push_back(kp);
            count++;
        }
    }
    
    Kmer_sorter sorter;   
    sort(kmer_descend_list.begin(), kmer_descend_list.end(), sorter);   
    shc_log_info(shc_logname, "Finish Sorting\n");    
}

/**
 * This function checks if the (K-1) suffix of a kmer has a common (k-1) prefix
 * with another kmer. 
 * @param kmer_n      : the kmer of interest, if there is another kmer, it is 
 *                      noted in the "info" field of the struct which can be
 *                      looked up by dictionary 
 * @param new_kmer_n  : Similarly to above, that this another kmer has (k-1) 
 *                      common prefix with another kmer, and is noted in "info".
 * @return            : 0 if this kmer has a suffix
 *                      -1 if this kmer does not have suffix
 */
uint8_t Kmer_handler::find_suffix_kmer(const uint64_t *kmer_n, uint64_t *new_kmer_n)
{
    char base_s[33];
    char new_base_s[33];
    uint64_t new_byte, best_byte;
    int count = 0;
    int max_count = -1;
    uint8_t best_base = DEFAULT_NUM;    
    
    write_all_prefix_info(kmer_n);
    
    decode_kmer(base_s, kmer_n, kmer_length);
    memcpy(new_base_s, base_s+1, kmer_length-1);
    for(int i=0; i<FOUR_BASE; i++)
    {         
        new_base_s[kmer_length-1] = num_to_char[i];
        encode_kmer(new_base_s, &new_byte, kmer_length);
        Kmer_counter_map_iterator it = kmer_counter.find(new_byte);                         
        //write_kmer_info(best_base, false, kmer_n);
        if (it != kmer_counter.end())
        {
            write_kmer_info(i, false, kmer_n);   //record this kmer its possible prefix string
            if (!it->second.used)
            {           
                count = it->second.count;
                //printf("count is %d\n", count);
                if (count > max_count)
                {
                    best_base = i;        
                    best_byte = new_byte;
                }             
            }            
        }        
    }
    
    if (best_base == DEFAULT_NUM)
    { 
        // so there is all kmer info entry is 0
        kmer_counter[*kmer_n].used = true;
        return NO_MATCH;
    }
    else
    {                
        kmer_counter[*kmer_n].used = true;
        // if there are num_contig contigs, the last contig is numbered as num_contig-1
        kmer_counter[*kmer_n].contig = ch->num_contig; 
        *new_kmer_n = best_byte;                                     
        kmer_counter[*new_kmer_n].used = true;   
        kmer_counter[*new_kmer_n].contig = ch->num_contig;
        return HAS_MATCH;
    }            
}

/**
 * This function checks if the (K-1) prefix of a kmer has a common (k-1) suffix
 * with another kmer. 
 * @param kmer_n      : the kmer of interest, if there is another kmer, it is 
 *                      noted in the "info" field of the struct which can be
 *                      looked up by dictionary 
 * @param new_kmer_n  : Similarly to above, that if this another kmer has (k-1) 
 *                      common suffix with the kmer of interest, then it is 
 *                      noted in "info" field.
 * @return            : 0 if this kmer has a prefix
 *                      -1 if this kmer does not have prefix
 */
uint8_t Kmer_handler::find_prefix_kmer(const uint64_t *kmer_n, uint64_t *new_kmer_n)
{
    char base_s[33];
    char new_base_s[33];
    uint64_t new_byte, best_byte;
    int count = 0;
    int max_count = -1;
    uint8_t best_base = DEFAULT_NUM;    
    
    write_all_suffix_info(kmer_n);
    
    //then work on finding its heaviest prefix
    decode_kmer(base_s, kmer_n, kmer_length);
    memcpy(new_base_s+1, base_s, kmer_length-1);
    for(int i=0; i<FOUR_BASE; i++)
    {
        //checking prefix kmer with four value
        new_base_s[0] = num_to_char[i];
        encode_kmer(new_base_s, &new_byte, kmer_length);
        Kmer_counter_map_iterator it = kmer_counter.find(new_byte); 
        
         //write_kmer_info(best_base, false, kmer_n);
        if (it != kmer_counter.end())
        {
            write_kmer_info(i, true, kmer_n);   //record this kmer its possible prefix string
            if(!it->second.used)
            {
                count = it->second.count;
                if (count > max_count)
                {
                    best_base = i;        
                    best_byte = new_byte;                    
                }
            }
        }
    }
    
    if (best_base == DEFAULT_NUM)
    {
        // so there is all kmer info entry is 0
        kmer_counter[*kmer_n].used = true;
        return NO_MATCH;
    }
    else
    {                
        kmer_counter[*kmer_n].used = true;
        kmer_counter[*kmer_n].contig = ch->num_contig;
        *new_kmer_n = best_byte;                                     
        kmer_counter[*new_kmer_n].used = true;
        kmer_counter[*new_kmer_n].contig = ch->num_contig;
        return HAS_MATCH;
    }            
}

void Kmer_handler::write_all_suffix_info(const uint64_t *kmer_num)
{
    char base_s[KMER_HOLDER_LEN];
    char new_base_s[KMER_HOLDER_LEN];
    uint64_t new_byte;
    Kmer_counter_map_iterator it;
    
    decode_kmer(base_s, kmer_num, kmer_length);
    memcpy(new_base_s, base_s+1, kmer_length-1);
    
    for(uint8_t i=0; i<FOUR_BASE; i++)
    {
        new_base_s[kmer_length-1] = num_to_char[i];
        encode_kmer(new_base_s, &new_byte, kmer_length);
        it = kmer_counter.find(new_byte); 
        if(it != kmer_counter.end())
        {
            //record this kmer its possible prefix string
            write_kmer_info(i, false, kmer_num);   
        }
    }    
}

void Kmer_handler::write_all_prefix_info(const uint64_t *kmer_num)
{
    char base_s[KMER_HOLDER_LEN];
    char new_base_s[KMER_HOLDER_LEN];
    uint64_t new_byte;
    Kmer_counter_map_iterator it;
    
    decode_kmer(base_s, kmer_num, kmer_length);    
    memcpy(new_base_s+1, base_s, kmer_length-1);
    
    for(uint8_t i=0; i<FOUR_BASE; i++)
    {
        new_base_s[0] = num_to_char[i];
        encode_kmer(new_base_s, &new_byte, kmer_length);
        it = kmer_counter.find(new_byte); 
        if(it != kmer_counter.end())
        {
            //record this kmer its possible prefix string
            write_kmer_info(i, true, kmer_num);   
        }
    }
}

/**
 * This function is the only method which changes the info field
 * @param num           : one numeric number for one of "A T C G "
 * @param is_prefix     : is it a prefix, true for yes, false for no
 * @param kmer_num      : kmer in numeric form
 */
void Kmer_handler::write_kmer_info(uint8_t num, bool is_prefix, const uint64_t *kmer_num)
{
    //char base_string[KMER_HOLDER_LEN];
    //base_string[kmer_length] ='\0';
    //decode_kmer(base_string, kmer_num, kmer_length);    
    uint8_t info = kmer_counter[*kmer_num].info;    
    if (is_prefix)
    {
        if(num==0)
            info |= (uint8_t)B1;
        else
            info = info | (((uint8_t)B1)<<num);            
    }
    else
    {        
        info = info | ((uint8_t)B1<<(PREFIX_OFFSET+num));
    }
    if(kmer_num != NULL)
        kmer_counter[*kmer_num].info = info;
    //else
        //print_bit(info);
}

/**
 * This function finds the contigs by going through all kmer in the dictionary.
 * For a specific kmer, it will find prefix first, then suffix. If it finds 
 * a prefix, it will then start with the kmer which shares common (k-1) 
 * character, and continue until there is no prefix kmer. Similarly for suffix.
 * At the end, it release all memory associated with sorting vector.
 * @return  number of contig
 */
contig_num_t Kmer_handler::find_contig()
{
    uint64_t next_kmer = 0;    
    char base_string[KMER_HOLDER_LEN];
    char root_string[KMER_HOLDER_LEN];
    base_string[kmer_length] = '\0';
    root_string[kmer_length] = '\0';
    uint32_t total_count = 0;
    uint32_t total_kmer_in_contig = 0;
    Contig_handler::size_type contig_len = 0;
    kmer_count_t contig_mean_count = 0;    
    uint8_t * contig_start;    
    
    shc_log_info(shc_logname, "Finding contig\n");    
    for(int i=0; i<kmer_descend_list.size(); i++)
    {  
        // start a new contig
        if (!kmer_counter[kmer_descend_list[i].first].used)                    
        {            
            contig_mean_count = 0;
            decode_kmer(root_string, &(kmer_descend_list[i].first), kmer_length);
            
            total_count = kmer_counter[kmer_descend_list[i].first].count;
            total_kmer_in_contig = 1;          
            //find prefix extension
            uint8_t re = find_prefix_kmer(&(kmer_descend_list[i].first), &next_kmer);
            uint64_t this_kmer = 0;
                    
            while (re == HAS_MATCH )
            { 
                ch->contig_list.push_back(decode_prefix_base(&next_kmer, kmer_length));                     
                total_count += kmer_counter[next_kmer].count;
                total_kmer_in_contig++;
#ifdef LOG_KMER                
                decode_kmer(base_string, &next_kmer, kmer_length);
                shc_log_info(shc_logname, "looking at kmer %s\n", base_string);   
#endif
                //print_bit(next_kmer);   
                this_kmer = next_kmer;
                re = find_prefix_kmer(&this_kmer, &next_kmer);                                
            }
            
            // get everything in order
            std::reverse( ch->contig_list.begin() + ch->delimitor.back(), 
                          ch->contig_list.end());            
                        
            // push the root string                            
            for(uint8_t t=0; t<kmer_length; t++)
                ch->contig_list.push_back(root_string[t]);
            
#ifdef LOG_KMER                
            shc_log_info(shc_logname, "Root kmer %s\n", root_string);   
#endif            
            // find suffix extension
            re = find_suffix_kmer(&(kmer_descend_list[i].first), &next_kmer);
            this_kmer = 0;
            
            while (re == HAS_MATCH )
            {
                //place where info is recorded
                ch->contig_list.push_back(decode_suffix_base(&next_kmer));                
                total_count += kmer_counter[next_kmer].count;
                total_kmer_in_contig++;
#ifdef LOG_KMER                
                decode_kmer(base_string, &next_kmer, kmer_length);  
                shc_log_info(shc_logname, "looking at kmer %s\n", base_string);                  
#endif                
                this_kmer = next_kmer;
                re = find_suffix_kmer(&this_kmer, &next_kmer);                
            }                                              
            contig_len = total_kmer_in_contig + kmer_length - 1;
            shc_log_info(shc_logname, "contig len %u\n", contig_len);
            
            contig_mean_count = (kmer_count_t)(total_count/total_kmer_in_contig);    
            
            
            //std::AAAAAAAAAAAAAAAAAAAAATCGGstring name = "one_contig.txt";
            //ch->dump_all_contig(name);
            //shc_log_info(shc_logname, "hello here\n");
            //ch->print_contig(ch->delimitor.back(), contig_len);
            
            
            if(decide_contig_and_build_rmer(contig_mean_count, contig_len))
            {
                //accept and upadte contig list info
                if(ch->delimitor[ch->num_contig]!=ch->delimitor.back())
                {
                    printf("No\n");                
                }
                
                ch->declare_new_contig(contig_mean_count, contig_len);                
            }
            else
            {
                //remove kmer list                
                contig_start = &(ch->contig_list.at(ch->delimitor[ch->num_contig]));    
                delete_kmer_for_contig(contig_start, contig_len);
                ch->reject_new_contig();                
            }                   
        }        
    }
    
    deallocate_kmer_descend_list();
    if(is_use_set)
    {
        deallocate_rmer_count_map();
    }
    else
    {
        deallocate_rmer_contig_map();
        deallocate_contig_count_map();
    }            
    
    //update the size, and release memory for kmer dict, see google sparsehash
    kmer_counter.resize(0);
    shc_log_info(shc_logname, "Finish finding contig, %ld contigs\n", 
                                                   ch->num_contig);
    return ch->num_contig;
}

uint8_t Kmer_handler::num_bit_info(uint8_t info)
{
    uint8_t num = (uint8_t)(B1) & info;
    
    for(uint8_t i=1; i<8; i++)
    {
        num += (info >> i) & (uint8_t)(B1);
    }
    return num;
}

void Kmer_handler::traverse_kmer_count()
{
    char temp[33];
    temp[kmer_length] = '\0';
    shc_log_info(shc_logname, "Start traversing kmer counter dict\n");
    Kmer_counter_map_iterator it;
    uint8_t num_bit = 255;
                    
    for (it = kmer_counter.begin(); it != kmer_counter.end(); it++) 
    {      
        num_bit = num_bit_info(it->second.info);
        //shc_log_info(shc_logname, "%u\n", num_bit);
        if(num_bit>2)
        {
            decode_kmer(temp, &(it->first), kmer_length);
            shc_log_info(shc_logname, "kmer %s in contig %u has more than two connectivity\n",
                                temp, it->second.contig);            
        }        
    }
    shc_log_info(shc_logname, "Finish traversing kmer counter dict\n");
}

/**
 * This function deallocates memory for sorting kmer vectors.
 */
void Kmer_handler::deallocate_kmer_descend_list()
{
    shc_log_info(shc_logname, "Start deallocate kmer sorting list\n");
    std::vector<Kmer_Occurence_Pair>().swap(kmer_descend_list);
    shc_log_info(shc_logname, "Finish deallocate kmer sorting list\n");
}

void Kmer_handler::deallocate_contig_count_map()
{
    shc_log_info(shc_logname, "Start deallocate contig count map for rmer list\n");
    Contig_count_map().swap(contig_count_map);
    shc_log_info(shc_logname, "Finish deallocate contig count map for rmer list\n");
}

void Kmer_handler::deallocate_rmer_contig_map()
{
    shc_log_info(shc_logname, "Start deallocate rmer contig map for rmer list\n");
    Rmer_contig_map().swap(rmer_contig_map);    
    shc_log_info(shc_logname, "Finish deallocate rmer contig map for rmer list\n");
}

void Kmer_handler::deallocate_rmer_count_map()
{
    shc_log_info(shc_logname, "Start deallocate rmer count map for set\n");
    Rmer_contig_map().swap(rmer_contig_map);    
    shc_log_info(shc_logname, "Finish deallocate rmer count map for set\n");
}

/**
 * The kmer are commented and is not considered used here for two reasons. 
 * 1. Clearing the kmer map does not reduce the memory, and map does not 
 * have a shrink_to_fit function.
 * 2. There are some problems to erase google sparsehash, it should have
 * a answer, but due to reason 1, it is not solved. When uncommenting, be aware.
 * @param i
 */
void Kmer_handler::delete_kmer_for_contig(uint8_t * contig_start, 
        Contig_handler::size_type contig_len)
{
    shc_log_info(shc_logname, "Start to delete kmer\n");
    uint64_t byte = 0;    
    char base_string[KMER_HOLDER_LEN];
    base_string[kmer_length] = '\0'; 
    
    for(Contig_handler::size_type i=0; i<contig_len-kmer_length+1; i++)
    {
        encode_kmer((char*)(contig_start+i), &byte, kmer_length);
        kmer_counter.erase(byte);        
    }                    
    shc_log_info(shc_logname, "Finish delete kmer\n");
}

void Kmer_handler::delete_contig_list(contig_num_t * list, contig_num_t num)
{
    ch->delete_contig_list(list, num);    
}
    
void Kmer_handler::get_contig_handler(Contig_handler * c_handler)
{
    ch = c_handler;
}

void Kmer_handler::get_logname(char * logname)
{
    shc_logname = logname;
}

/**
 * This function first checks if using a count or a list, then use contig length
 * and count to do a primary filter. If it passes, then check if it shares 
 * common rmer with previous set. An exception is that if it is the first 
 * rmer, we always accept the contig.
 * @param mean_count
 * @param contig len
 * @return if this contig is accepted, if not rmer is not built
 */
bool Kmer_handler::decide_contig_and_build_rmer(kmer_count_t mean_count, Contig_handler::size_type len)
{
    shc_log_info(shc_logname, "Start contig decision \t\t\t CANDIDATE CONTIG %d\n", ch->num_contig+1);        
    shc_log_info(shc_logname, "Start length count filter\n");
    if(len<min_len || mean_count<min_count)
    {
        shc_log_info(shc_logname, "Finish length count filter, fail to pass, due to length count\n");
        return false;                
    }
    shc_log_info(shc_logname, "Finish length count filter, Passing\n");
    
    if(is_use_set)
    {
        return use_set_to_filter(mean_count, len);
    }
    else
    {
        return use_list_to_filter(mean_count, len);
    }
}

bool Kmer_handler::use_set_to_filter(kmer_count_t mean_count, Contig_handler::size_type len)
{
    shc_log_info(shc_logname, "Start set filter\n");
    uint8_t * contig_start = &(ch->contig_list.at(ch->delimitor[ch->num_contig]));        
    char temp[32];    
    temp[rmer_len] = '\0';
    uint64_t byte;    
    if(rmer_count_map.empty())
    {
        for(Contig_handler::size_type i=0; i<len-rmer_len+1; i++)
        {                
            encode_kmer((char*)(contig_start+i), &byte, rmer_len);
            rmer_count_map[byte] = 1;
        }      
        shc_log_info(shc_logname, "Finish set filter, first contig\n");
        return true;
    }
    else
    {        
        uint64_t common_element = 0;
        Contig_handler::size_type total_rmer_num = len-rmer_len+1;                                    
        
        shc_log_info(shc_logname, "Calculating common element stat\n");
        for(Contig_handler::size_type i=0; i<total_rmer_num; i++)
        {                                                
            encode_kmer((char*)(contig_start+i), &byte, rmer_len);

            //memcpy(temp,(contig_start+i), rmer_len);               
            //shc_log_info(shc_logname, "%s\n", temp);

            if(rmer_count_map.find(byte) != rmer_count_map.end())
                common_element++;                
        }

        shc_log_info(shc_logname, "\t\t\tSTAT summary\n");
        shc_log_info(shc_logname, "\t\t\tcontig length %d\n", len);            
        shc_log_info(shc_logname, "\t\t\tCommon element %lld\n", common_element);
        shc_log_info(shc_logname, "\t\t\ttotal_rmer_num %lld\n", total_rmer_num);


        shc_log_info(shc_logname, "Start redundancy filter\n");
        if(static_cast<double>(common_element)/total_rmer_num < r_thresh)
        {
            for(Contig_handler::size_type i=0; i<total_rmer_num; i++)
            {                   
                encode_kmer((char*)(contig_start+i), &byte, rmer_len);
                if(rmer_count_map.find(byte) != rmer_count_map.end())
                    rmer_count_map[byte]++ ;                
                else
                    rmer_count_map[byte] = 1;                    
            }
            shc_log_info(shc_logname, "Finish redundancy filter, Passing\n");
            return true;
        }
        else
        {
            shc_log_info(shc_logname, "Finish redundancy filter, Fail to pass due to high redundance\n");
            return false;
        }            
    }                
}

bool Kmer_handler::use_list_to_filter(kmer_count_t mean_count, Contig_handler::size_type len)
{
    shc_log_info(shc_logname, "Start list filter\n");
    uint8_t * contig_start = &(ch->contig_list.at(ch->delimitor[ch->num_contig]));        
    char temp[32];    
    temp[rmer_len] = '\0';
    uint64_t byte;        
    Contig_handler::size_type total_num_rmer = len-rmer_len+1;
    if(rmer_contig_map.empty())
    {
        for(Contig_handler::size_type i=0; i<total_num_rmer; i++)
        {                
            encode_kmer((char*)(contig_start+i), &byte, rmer_len);
            rmer_contig_map[byte].push_back(0);
        }      
        shc_log_info(shc_logname, "Finish contig decision, first contig\n");
        return true;
    }
    else
    {
        shc_log_info(shc_logname, "Start length count filter\n");
        uint64_t common_element = 0;
        Contig_handler::size_type total_rmer_num = len-rmer_len+1;                                    
               
        shc_log_info(shc_logname, "Creating contig count filter\n");
        for(Contig_handler::size_type i=0; i<total_rmer_num; i++)
        {                      
            //get rmer            
            encode_kmer((char*)(contig_start+i), &byte, rmer_len);
            //check the count and record in a dict
            for(std::vector<contig_num_t>::iterator it=rmer_contig_map[byte].begin();
                                               it != rmer_contig_map[byte].end();
                                               it++)
            {
                contig_count_map[*it]++;
            }
            //check dict to see redundancy
            Contig_count_map_const_iterator it;
            for(it=contig_count_map.begin(); it!=contig_count_map.end(); it++)
            {
                double thresh = static_cast<double>((*it).second)/total_num_rmer;
                
                if (thresh >= r_thresh)
                {
                    shc_log_info(shc_logname, "Finish list filter, not Passing due to high redundancy\n");
                    contig_count_map.clear();
                    return false;
                }
                else
                {
                    //update rmer_contig_map
                    for(Contig_handler::size_type i=0; i<total_num_rmer; i++)
                    {                
                        encode_kmer((char*)(contig_start+i), &byte, rmer_len);
                        //since the current count has not been updated
                        //also notice that if the accepted contig contains
                        //the same kmer multiple times, the for the contig 
                        //list that corresponds to the kmer would contain
                        //three element with the contig number
                        //Change it if needed
                        rmer_contig_map[byte].push_back(ch->num_contig+1);     
                    }    
                    shc_log_info(shc_logname, "Finish list filter, Passing\n");
                    contig_count_map.clear();
                    return true;
                }
            }                        
        }                    
    }    
}



/**
    
    */
/*
 shc_log_info(shc_logname, "Start Filtering contigs\n");
    
    std::vector<contig_num_t> remove_list;
    for(contig_num_t i=0; i<num_contig; i++)
    {
        size_type len = delimitor[i+1] - delimitor[i];
        if(len<min_len || mean_count[i]<min_count)
        {
            shc_log_info(shc_logname, "Remove %u \n", i);
            remove_list.push_back(i);
        }
    }        
                
    if(!remove_list.empty())
    {
        get_vector_string(remove_list, vec_str);
        shc_log_info(shc_logname, "Deleting vec %s\n", vec_str.str().c_str());
        delete_contig_list( &(remove_list.at(0)), remove_list.size());            
    }
    else    
    {
        shc_log_info(shc_logname, "length count checks and Nothing to delete\n");    
    }
    
    remove_redundance(threshold);
    
    std::vector<contig_num_t>().swap(remove_list);
    shc_log_info(shc_logname, "Finish Filtering contigs\n");
 */