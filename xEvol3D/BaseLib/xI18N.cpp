/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "../xStdPch.h"
#include "xI18N.h"
#include <libcharset.h>
#include <string>
#include <vector>
#include <wchar.h>
using namespace std;
#ifdef _WIN32
#define CP_UTF16 1200
#define CP_UTF32 12000
#define CP_UTF32BE 12001
#endif

BEGIN_NAMESPACE_XEVOL3D




xI18N::xI18N()
{
#ifdef _WIN32
    add(0x0436, L"Afrikaans (South Africa)"                        ,L"af_ZA"        ,   L"Latn"                ,1252);  
    add(0x041c, L"Albanian (Albania)"                              ,L"sq_AL"        ,   L"Latn"                ,1252);  
    add(0x0484, L"Alsatian (France)"                               ,L"gsw_FR"       ,   L"Alsatian"            ,  -1);    
    add(0x045e, L"Amharic (Ethiopia)"                              ,L"am_ET"        ,   L"Amharic"             ,  -1);
    add(0x1401, L"Arabic (Algeria)"                                ,L"ar_DZ"        ,   L"Arabic"              ,1256); 
    add(0x3c01, L"Arabic (Bahrain)"                                ,L"ar_BH"        ,   L"Arabic"              ,1256); 
    add(0x0c01, L"Arabic (Egypt)"                                  ,L"ar_EG"        ,   L"Arabic"              ,1256); 
    add(0x0801, L"Arabic (Iraq)"                                   ,L"ar_IQ"        ,   L"Arabic"              ,1256); 
    add(0x2c01, L"Arabic (Jordan)"                                 ,L"ar_JO"        ,   L"Arabic"              ,1256); 
    add(0x3401, L"Arabic (Kuwait)"                                 ,L"ar_KW"        ,   L"Arabic"              ,1256); 
    add(0x3001, L"Arabic (Lebanon)"                                ,L"ar_LB"        ,   L"Arabic"              ,1256); 
    add(0x1001, L"Arabic (Libya)"                                  ,L"ar_LY"        ,   L"Arabic"              ,1256); 
    add(0x1801, L"Arabic (Morocco)"                                ,L"ar_MA"        ,   L"Arabic"              ,1256); 
    add(0x2001, L"Arabic (Oman)"                                   ,L"ar_OM"        ,   L"Arabic"              ,1256); 
    add(0x4001, L"Arabic (Qatar)"                                  ,L"ar_QA"        ,   L"Arabic"              ,1256); 
    add(0x0401, L"Arabic (Saudi Arabia)"                           ,L"ar_SA"        ,   L"Arabic"              ,1256); 
    add(0x2801, L"Arabic (Syria)"                                  ,L"ar_SY"        ,   L"Arabic"              ,1256); 
    add(0x1c01, L"Arabic (Tunisia)"                                ,L"ar_TN"        ,   L"Arabic"              ,1256); 
    add(0x3801, L"Arabic (U.A.E.)"                                 ,L"ar_AE"        ,   L"Arabic"              ,1256); 
    add(0x2401, L"Arabic (Yemen)"                                  ,L"ar_YE"        ,   L"Arabic"              ,1256); 
    add(0x042b, L"Armenian (Armenia)"                              ,L"hy_AM"        ,   L"Armenian"            ,  -1);
    add(0x044d, L"Assamese (India)"                                ,L"as_IN"        ,   L"Assamese"            ,  -1); 
    add(0x082c, L"Azeri (Azerbaijan, Cyrillic)"                    ,L"az_Cyrl_AZ"   ,   L"Azeri"               ,1251); 
    add(0x042c, L"Azeri (Azerbaijan, Latin)"                       ,L"az_Latn_AZ"   ,   L"Azeri"               ,1254); 
    add(0x046d, L"Bashkir (Russia)"                                ,L"ba_RU"        ,   L"Bashkir"             ,  -1);          
    add(0x042d, L"Basque (Basque)"                                 ,L"eu_ES"        ,   L"Basque"              ,1252); 
    add(0x0423, L"Belarusian (Belarus)"                            ,L"be_BY"        ,   L"Belarusian"          ,1251); 
    add(0x0445, L"Bengali (India)"                                 ,L"bn_IN"        ,   L"Bengali"             ,  -1);
    add(0x201a, L"Bosnian (Bosnia Herzegovina, Cyrillic)"          ,L"bs_Cyrl_BA"   ,   L"Bosnian"             ,1251); 
    add(0x141a, L"Bosnian (Bosnia Herzegovina, Latin)"             ,L"bs_Latn_BA"   ,   L"Bosnian"             ,1250); 
    add(0x047e, L"Breton (France)"                                 ,L"br_FR"        ,   L"Breton"              ,1252); 
    add(0x0402, L"Bulgarian (Bulgaria)"                            ,L"bg_BG"        ,   L"Bulgarian"           ,1251); 
    //0x0455 NotL supported: Burmese                                L                   L            
    add(0x0403 ,L"Catalan (Catalan)"                               ,L"ca_ES"        ,   L"Catalan"             ,1252); 
    add(0x0c04 ,L"Chinese (Hong Kong SAR, PRC)"                    ,L"zh_HK"        ,   L"Chinese(HK)"         ,950 );
    add(0x1404 ,L"Chinese (Macao SAR)"                             ,L"zh_MO"        ,   L"Chinese(TW)"         ,950 );
    add(0x0804 ,L"Chinese (PRC)"                                   ,L"zh_CN"        ,   L"Chinese"             ,936 );
    add(0x1004 ,L"Chinese (Singapore)"                             ,L"zh_SG"        ,   L"Chinese"             ,936 );
    add(0x0404 ,L"Chinese (Taiwan)"                                ,L"zh_TW"        ,   L"Chinese(TW)"         ,950 );
    //add(CorsicLan (France) co_FR                                                                 
    add(0x101a , L"Croatian (Bosnia and Herzegovina, Latin)"       ,L"hr_BA"        ,   L"Croatian"            ,1250); 
    add(0x041a , L"Croatian (Croatia)"                             ,L"hr_HR"        ,   L"Croatian"            ,1250); 
    add(0x0405 , L"Czech (Czech Republic)"                         ,L"cs_CZ"        ,   L"Czech"               ,1250); 
    add(0x0406 , L"Danish (Denmark)"                               ,L"da_DK"        ,   L"Danish"              ,1252); 
    add(0x048c , L"Dari (Afghanistan)"                             ,L"gbz_AF"       ,   L"Dari"                ,1256); 
    add(0x0465 , L"Divehi (Maldives)"                              ,L"dv_MV"        ,   L"Divehi"              ,-1  );
    add(0x0813 , L"Dutch (Belgium)"                                ,L"nl_BE"        ,   L"Dutch"               ,1252); 
    add(0x0413 , L"Dutch (Netherlands)"                            ,L"nl_NL"        ,   L"Dutch"               ,1252); 
    add(0x0c09 , L"English (Australia)"                            ,L"en_AU"        ,   L"English"             ,1252); 
    add(0x2809 , L"English (Belize)"                               ,L"en_BZ"        ,   L"English"             ,1252); 
    add(0x1009 , L"English (Canada)"                               ,L"en_CA"        ,   L"English"             ,1252); 
    add(0x2409 , L"English (Caribbean)"                            ,L"en_029"       ,   L"English"             ,1252); 
    add(0x4009 , L"English (India)"                                ,L"en_IN"        ,   L"English"             ,1252); 
    add(0x1809 , L"English (Ireland)"                              ,L"en_IE"        ,   L"English"             ,1252); 
    add(0x2009 , L"English (Jamaica)"                              ,L"en_JM"        ,   L"English"             ,1252); 
    add(0x4409 , L"English (Malaysia)"                             ,L"en_MY"        ,   L"English"             ,1252); 
    add(0x1409 , L"English (New Zealand)"                          ,L"en_NZ"        ,   L"English"             ,1252); 
    add(0x3409 , L"English (Philippines)"                          ,L"en_PH"        ,   L"English"             ,1252); 
    add(0x4809 , L"English (Singapore)"                            ,L"en_SG"        ,   L"English"             ,1252); 
    add(0x1c09 , L"English (South Africa)"                         ,L"en_ZA"        ,   L"English"             ,1252); 
    add(0x2c09 , L"English (Trinidad and Tobago)"                  ,L"en_TT"        ,   L"English"             ,1252); 
    add(0x0809 , L"English (United Kingdom)"                       ,L"en_GB"        ,   L"English"             ,1252); 
    add(0x0409 , L"English (United States)"                        ,L"en_US"        ,   L"English"             ,1252); 
    add(0x3009 , L"English (Zimbabwe)"                             ,L"en_ZW"        ,   L"English"             ,1252); 
    add(0x0425 , L"Estonian (Estonia)"                             ,L"et_EE"        ,   L"Faroese"             ,1257); 
    add(0x0438 , L"Faroese (Faroe Islands)"                        ,L"fo_FO"        ,   L"Filipino"            ,1252); 
    add(0x0464 , L"Filipino (Philippines)"                         ,L"fil_PH"       ,   L"Filipino"            ,1252); 
    add(0x040b , L"Finnish (Finland)"                              ,L"fi_FI"        ,   L"Finnish"             ,1252); 
    add(0x080c , L"French (Belgium)"                               ,L"fr_BE"        ,   L"French"              ,1252); 
    add(0x0c0c , L"French (Canada)"                                ,L"fr_CA"        ,   L"French"              ,1252); 
    add(0x040c , L"French (France)"                                ,L"fr_FR"        ,   L"French"              ,1252); 
    add(0x140c , L"French (Luxembourg)"                            ,L"fr_LU"        ,   L"French"              ,1252); 
    add(0x180c , L"French (Monaco)"                                ,L"fr_MC"        ,   L"French"              ,1252); 
    add(0x100c , L"French (Switzerland)"                           ,L"fr_CH"        ,   L"French"              ,1252); 
    add(0x0462 , L"Frisian (Netherlands)"                          ,L"fy_NL"        ,   L"Frisian"             ,1252); 
    add(0x0456 , L"Galician (Spain)"                               ,L"gl_ES"        ,   L"Galician"            ,1252); 
    add(0x0437 , L"Georgian (Georgia)"                             ,L"ka_GE"        ,   L"Georgian"            ,-1  );
    add(0x0c07 , L"German (Austria)"                               ,L"de_AT"        ,   L"German"              ,1252); 
    add(0x0407 , L"German (Germany)"                               ,L"de_DE"       ,   L"German"              ,1252);                                                    
    add(0x1407 , L"German (Liechtenstein)"                         ,L"de_LI"       ,   L"German"              ,1252); 
    add(0x1007 , L"German (Luxembourg)"                            ,L"de_LU"       ,   L"German"              ,1252); 
    add(0x0807 , L"German (Switzerland)"                           ,L"de_CH"       ,   L"German"              ,1252); 
    add(0x0408 , L"Greek (Greece)"                                 ,L"el_GR"       ,   L"Greek"               ,1253); 
    add(0x046f , L"Greenlandic (Greenland)"                        ,L"kl_GL"       ,   L"Greenlandic"         ,1252); 
    add(0x0447 , L"Gujarati (India)"                               ,L"gu_IN"       ,   L"Gujarati"            ,  -1); 
    add(0x0468 , L"Hausa (Nigeria, Latin)"                         ,L"ha_Latn_NG"  ,   L"Hausa"               ,1252); 
    add(0x040d , L"Hebrew (Israel)"                                ,L"he_IL"       ,   L"Hebrew"              ,1255); 
    add(0x0439 , L"Hindi (India)"                                  ,L"hi_IN"       ,   L"Deva"                ,  -1); 
    add(0x040e , L"Hungarian (Hungary)"                            ,L"hu_HU"       ,   L"Hindi"               ,1250); 
    add(0x040f , L"Icelandic (Iceland)"                            ,L"is_IS"       ,   L"Icelandic"           ,1252); 
    add(0x0470 , L"Igbo (Nigeria)"                                 ,L"ig_NG"       ,   L"Nigeria"             ,  -1);
    add(0x0421 , L"Indonesian (Indonesia)"                         ,L"id_ID"       ,   L"Indonesian"          ,1252); 
    add(0x085d , L"Inuktitut (Canada, Latin)"                      ,L"iu_Latn_CA"  ,   L"Inuktitut"           ,1252); 
    add(0x045d , L"Inuktitut (Canada, Syllabics)"                  ,L"iu_Cans_CA"  ,   L"Inuktitut"           ,  -1); 
    add(0x083c , L"Irish (Ireland)"                                ,L"ga_IE"       ,   L"Irish"               ,1252); 
    add(0x0410 , L"Italian (Italy)"                                ,L"it_IT"       ,   L"Italian"             ,1252); 
    add(0x0810 , L"Italian (Switzerland)"                          ,L"it_CH"       ,   L"Italian"             ,1252); 
    add(0x0411 , L"Japanese (Japan)"                               ,L"ja_JP"       ,   L"Japanese"            ,932 );                                     
    add(0x044b , L"Kannada (India)"                                ,L"kn_IN"       ,   L"Kannada"             ,-1  );
    add(0x043f , L"Kazakh (Kazakhstan)"                            ,L"kk_KZ"       ,   L"Kazakh"              ,1251); 
    add(0x0453 , L"Khmer (Cambodia)"                               ,L"kh_KH"       ,   L"Khmr"                ,  -1);
    add(0x0486 , L"K'iche (Guatemala)"                             ,L"qut_GT"      ,   L"Cambodia"            ,1252);
    add(0x0487 , L"Kinyarwanda (Rwanda)"                           ,L"rw_RW"       ,   L"Kinyarwanda"         ,1252);
    add(0x0457 , L"Konkani (India)"                                ,L"kok_IN"      ,   L"Konkani"             ,  -1);
    add(0x0812 , L"Korean (Johab)"                                 ,L"ko_KR"       ,   L"Korean"              ,949 ); 
    add(0x0412 , L"Korean (Korea)"                                 ,L"ko_KR"       ,   L"Korean"              ,949 );
    add(0x0440 , L"Kyrgyz (Kyrgyzstan)"                            ,L"ky_KG"       ,   L"Kyrgyzstan"          ,1251);
    add(0x0454 , L"Lao (Lao PDR)"                                  ,L"lo_LA"       ,   L"Lao"                 ,-1  );
    add(0x0426 , L"Latvian (Latvia)"                               ,L"lv_LV"       ,   L"Latvian"             ,1257);
    add(0x0427 , L"Lithuanian (Lithuania)"                         ,L"lt_LT"       ,   L"Lithuanian"          ,1257);
    add(0x082e , L"Sorbian (Germany)"                              ,L"dsb_DE"      ,   L"Sorbian"             ,1252);
    add(0x046e , L"Luxembourgish (Luxembourg)"                     ,L"lb_LU"       ,   L"Luxembourgish"       ,1252);
    add(0x042f , L"Macedonian (Macedonia, FYROM)"                  ,L"mk_MK"       ,   L"Macedonian"          ,1251);                                       
    add(0x083e , L"Malay (Brunei Darussalam)"                      ,L"ms_BN"       ,   L"Darussalam"          ,1252); 
    add(0x043e , L"Malay (Malaysia)"                               ,L"ms_MY"       ,   L"Malaysia"            ,1252); 
    add(0x044c , L"Malayalam (India)"                              ,L"ml_IN"       ,   L"Malayalam"           ,  -1); 
    add(0x043a , L"Maltese (Malta)"                                ,L"mt_MT"       ,   L"Maltese"             ,1252); 
    add(0x0481 , L"Maori (New Zealand)"                            ,L"mi_NZ"       ,   L"Maori"               ,1252); 
    add(0x047a , L"Mapudungun (Chile)"                             ,L"arn_CL"      ,   L"Mapudungun"          ,1252); 
    add(0x044e , L"Marathi (India)"                                ,L"mr_IN"       ,   L"Marathi"             ,  -1); 
    add(0x047c , L"Mohawk (Canada)"                                ,L"moh_CA"      ,   L"Mohawk"              ,1252); 
    add(0x0450 , L"Mongolian (Mongolia)"                           ,L"mn_Cyrl_MN"  ,   L"Mongolian"           ,1251); 
    add(0x0850 , L"Mongolian (PRC)"                                ,L"mn_Mong_CN"  ,   L"Mongolian"           ,  -1); 
    //add(       L  "Nepali (India)"                                L, "ne_IN"       , L  ""                  , -1 );
    add(0x0461 , L"Nepali (Nepal)"                                 ,L"ne_NP"       ,   L"Nepali"              ,  -1); 
    add(0x0414 , L"Norwegian (Bokmål, Norway)"                     ,L"nb_NO"       ,   L"Norwegian"           ,1252); 
    add(0x0814 , L"Norwegian (Nynorsk, Norway)"                    ,L"nn_NO"       ,   L"Norwegian"           ,1252); 
    add(0x0482 , L"Occitan (France)"                               ,L"oc_FR"       ,   L"Occitan"             ,1252); 
    add(0x0448 , L"Oriya (India)"                                  ,L"or_IN"       ,   L"Oriya"               ,  -1); 
    add(0x0463 , L"Pashto (Afghanistan)"                           ,L"ps_AF"       ,   L"Pashto"              ,  -1);
    add(0x0429 , L"Persian (Iran)"                                 ,L"fa_IR"       ,   L"Persian"             ,1256); 
    add(0x0415 , L"Polish (Poland)"                                ,L"pl_PL"       ,   L"Polish"              ,1250); 
    add(0x0416 , L"Portuguese (Brazil)"                            ,L"pt_BR"       ,   L"Portuguese"          ,1252); 
    add(0x0816 , L"Portuguese (Portugal)"                          ,L"pt_PT"       ,   L"Portuguese"          ,1252); 
    add(0x0446 , L"Punjabi (India)"                                ,L"pa_IN"       ,   L"Punjabi"             ,  -1); 
    add(0x046b , L"Quechua (Bolivia)"                              ,L"quz_BO"      ,   L"Quechua"             ,1252); 
    add(0x086b , L"Quechua (Ecuador)"                              ,L"quz_EC"      ,   L"Quechua"             ,1252); 
    add(0x0c6b , L"Quechua (Peru)"                                 ,L"quz_PE"      ,   L"Quechua"             ,1252); 
    add(0x0418 , L"Romanian (Romania)"                             ,L"ro_RO"       ,   L"Romanian"            ,1250); 
    add(0x0417 , L"Romansh (Switzerland)"                          ,L"rm_CH"       ,   L"Romansh"             ,1252); 
    add(0x0419 , L"Russian (Russia)"                               ,L"ru_RU"       ,   L"Russian"             ,1251); 
    add(0x243b , L"Sami (Inari, Finland)"                          ,L"smn_FI"      ,   L"Sami"                ,1252); 
    add(0x103b , L"Sami (Lule, Norway)"                            ,L"smj_NO"      ,   L"Sami"                ,1252); 
    add(0x143b , L"Sami (Lule, Sweden)"                            ,L"smj_SE"      ,   L"Sami"                ,1252); 
    add(0x0c3b , L"Sami (Northern, Finland)"                       ,L"se_FI"       ,   L"Sami"                ,1252); 
    add(0x043b , L"Sami (Northern, Norway)"                        ,L"se_NO"       ,   L"Sami"                ,1252); 
    add(0x083b , L"Sami (Northern, Sweden)"                        ,L"se_SE"       ,   L"Sami"                ,1252); 
    add(0x203b , L"Sami (Skolt, Finland)"                          ,L"sms_FI"      ,   L"Sami"                ,1252); 
    add(0x183b , L"Sami (Southern, Norway)"                        ,L"sma_NO"      ,   L"Sami"                ,1252); 
    add(0x1c3b , L"Sami (Southern, Sweden)"                        ,L"sma_SE"      ,   L"Sami"                ,1252); 
    add(0x044f , L"Sanskrit (India)"                               ,L"sa_IN"       ,   L"Sanskrit"            ,  -1); 
    add(0x1c1a , L"Serbian (Bosnia and Herzegovina, Cyrillic)"     ,L"sr_Cyrl_BA"  ,   L"Serbian"             ,1251); 
    add(0x181a , L"Serbian (Bosnia and Herzegovina, Latin)"        ,L"sr_Latn_BA"  ,   L"Serbian"             ,1250); 
    add(0x0c1a , L"Serbian (Serbia, Cyrillic)"                     ,L"sr_Cyrl_CS"  ,   L"Serbian"             ,1251); 
    add(0x0816 , L"Portuguese (Portugal)"                          ,L"pt_PT"       ,   L"Portuguese"          ,1252); 
    add(0x081a , L"Serbian (Serbia, Latin)"                        ,L"sr_Latn_CS"  ,   L"Serbian"             ,1250); 
    add(0x046c , L"Sesotho sa Leboa/Northern Sotho (South Africa)" ,L"ns_ZA"       ,   L"Sesotho"             ,1252);  
    add(0x0432 , L"Setswana/Tswana (South Africa)"                 ,L"tn_ZA"       ,   L"Setswana"            ,1252); 
    add(0x045b , L"Sinhala (Sri Lanka)"                            ,L"si_LK"       ,   L"Sinhala"             ,  -1); 
    add(0x041b , L"Slovak (Slovakia)"                              ,L"sk_SK"       ,   L"Slovak"              ,1250); 
    add(0x0424 , L"Slovenian (Slovenia)"                           ,L"sl_SI"       ,   L"Slovenian"           ,1250); 
    add(0x2c0a , L"Spanish (Argentina)"                            ,L"es_AR"       ,   L"Spanish"             ,1252); 
    add(0x400a , L"Spanish (Bolivia)"                              ,L"es_BO"       ,   L"Spanish"             ,1252); 
    add(0x340a , L"Spanish (Chile)"                                ,L"es_CL"       ,   L"Spanish"             ,1252); 
    add(0x240a , L"Spanish (Colombia)"                             ,L"es_CO"       ,   L"Spanish"             ,1252); 
    add(0x140a , L"Spanish (Costa Rica)"                           ,L"es_CR"       ,   L"Spanish"             ,1252); 
    add(0x1c0a , L"Spanish (Dominican Republic)"                   ,L"es_DO"       ,   L"Spanish"             ,1252); 
    add(0x300a , L"Spanish (Ecuador)"                              ,L"es_EC"       ,   L"Spanish"             ,1252); 
    add(0x440a , L"Spanish (El Salvador)"                          ,L"es_SV"       ,   L"Spanish"             ,1252); 
    add(0x100a , L"Spanish (Guatemala)"                            ,L"es_GT"       ,   L"Spanish"             ,1252); 
    add(0x480a , L"Spanish (Honduras)"                             ,L"es_HN"       ,   L"Spanish"             ,1252); 
    add(0x080a , L"Spanish (Mexico)"                               ,L"es_MX"       ,   L"Spanish"             ,1252); 
    add(0x4c0a , L"Spanish (Nicaragua)"                            ,L"es_NI"       ,   L"Spanish"             ,1252); 
    add(0x180a , L"Spanish (Panama)"                               ,L"es_PA"       ,   L"Spanish"             ,1252); 
    add(0x3c0a , L"Spanish (Paraguay)"                             ,L"es_PY"       ,   L"Spanish"             ,1252); 
    add(0x280a , L"Spanish (Peru)"                                 ,L"es_PE"       ,   L"Spanish"             ,1252); 
    add(0x500a , L"Spanish (Puerto Rico)"                          ,L"es_PR"       ,   L"Spanish"             ,1252); 
    add(0x0c0a , L"Spanish (Spain)"                                ,L"es_ES"       ,   L"Spanish"             ,1252); 
    add(0x040a , L"Spanish (Spain, Traditional Sort)"              ,L"es_ES_tradnl",   L"Spanish"             ,1252); 
    add(0x540a , L"Spanish (United States)"                        ,L"es_US"       ,   L"Spanish"             ,1252); 
    add(0x380a , L"Spanish (Uruguay)"                              ,L"es_UY"       ,   L"Spanish"             ,1252); 
    add(0x200a , L"Spanish (Venezuela)"                            ,L"es_VE"       ,   L"Spanish"             ,1252); 
    //add(0x043, L"0 Not supported: Sutu                            L                  L                     
    add(0x0441 , L"Swahili (Kenya)"                                ,L"sw_KE"       ,   L"Swahili"             ,1252); 
    add(0x081d , L"Swedish (Finland)"                              ,L"sv_FI"       ,   L"Swedish"             ,1252); 
    add(0x041d , L"Swedish (Sweden)"                               ,L"sv_SE"       ,   L"Swedish"             ,1252); 
    add(0x045a , L"Syriac (Syria)"                                 ,L"syr_SY"      ,   L"Syriac"              ,  -1); 
    add(0x0428 , L"Tajik (Tajikistan)"                             ,L"tg_Cyrl_TJ"  ,   L"Tajikistan"          ,1251); 
    add(0x085f , L"Tamazight (Algeria, Latin)"                     ,L"tmz_Latn_DZ" ,   L"Tamazight"           ,1252); 
    add(0x0449 , L"Tamil (India)"                                  ,L"ta_IN"       ,   L"Tamil"               ,  -1); 
    add(0x0444 , L"Tatar (Russia)"                                 ,L"tt_RU"       ,   L"Tatar"               ,1251); 
    add(0x044a , L"Telugu (India)"                                 ,L"te_IN"       ,   L"Telugu"              ,  -1); 
    add(0x041e , L"Thai (Thailand)"                                ,L"th_TH"       ,   L"Thailand"            , 874);
    add(0x0851 , L"Tibetan (Bhutan)"                               ,L"bo_BT"       ,   L"Tibetan"             ,  -1); 
    add(0x0451 , L"Tibetan (PRC)"                                  ,L"bo_CN"       ,   L"Tibetan"             ,  -1); 
    add(0x041f , L"Turkish (Turkey)"                               ,L"tr_TR"       ,   L"Turkish"             ,1254); 
    add(0x0442 , L"Turkmen (Turkmenistan)"                         ,L"tk_TM"       ,   L"Turkmen"             ,1251); 
    add(0x0480 , L"Uighur (PRC)"                                   ,L"ug_CN"       ,   L"Uighur"              ,1256); 
    add(0x0422 , L"Ukrainian (Ukraine)"                            ,L"uk_UA"       ,   L"Ukrainian"           ,1251); 
    add(0x042e , L"Sorbian (Germany)"                              ,L"wen_DE"      ,   L"Sorbian"             ,1252); 
    add(0x0820 , L"Urdu (India)"                                   ,L"tr_IN"       ,   L"Urdu"                ,  -1);
    add(0x0420 , L"Urdu (Pakistan)"                                ,L"ur_PK"       ,   L"Pakistan"            ,1256); 
    add(0x0843 , L"Uzbek (Uzbekistan, Cyrillic)"                   ,L"uz_Cyrl_UZ"  ,   L"Uzbek"               ,1251); 
    add(0x0443 , L"Uzbek (Uzbekistan, Latin)"                      ,L"uz_Latn_UZ"  ,   L"Uzbek"               ,1254); 
    add(0x042a , L"Vietnamese (Vietnam)"                           ,L"vi_VN"       ,   L"Vietnamese"          ,1258); 
    add(0x0452 , L"Welsh (United Kingdom)"                         ,L"cy_GB"       ,   L"Welsh"               ,1252); 
    add(0x0488 , L"Wolof (Senegal)"                                ,L"wo_SN"       ,   L"Wolof"               ,1252); 
    add(0x0434 , L"Xhosa/isiXhosa (South Africa)"                  ,L"xh_ZA"       ,   L"Xhosa"               ,1252); 
    add(0x0485 , L"Yakut (Russia)"                                 ,L"sah_RU"       ,  L"Yakut"               ,1251); 
    add(0x0478 , L"Yi (PRC)"                                       ,L"ii_CN"       ,   L"Yi"                  ,  -1); 
    add(0x046a , L"Yoruba (Nigeria)"                               ,L"yo_NG"       ,   L"Yoruba"              ,  -1);
    add(0x0435 , L"Zulu/isiZulu (South Africa)"                    ,L"zu_ZA"       ,   L"Zulu"                ,1252); 
#else                                                                                          

#endif                                                                                                                                               
}                                                                                                                                          

xI18N::~xI18N()
{

}

#ifdef _WIN32      
void   xI18N::add(LANGID _id , const wchar_t* _langDispName, const wchar_t* _locale, const wchar_t* _langName , int cpNumber)                                                   
{                                                                                                                                          
    xLocaleInfo_t info;
    info._langDispName = _langDispName;
    info._langID       = _id;
    info._locale       = _locale;
    info._langName     = _langName;
    info._cpNumber     = cpNumber;
    m_locals.push_back(info);      
} 

xI18N::xLocaleInfo_t* xI18N::getByLangID(LANGID langID)
{            
	size_t nLocales = m_locals.size();
    for(size_t i = 0 ; i <  nLocales; ++i)
    {        
        if(m_locals[i]._langID == langID)
            return &m_locals[i];
    }        
    return NULL;
} 

xI18N::xLocaleInfo_t* xI18N::getCurrentLocaleInfor()
{
      LANGID lanID = ::GetUserDefaultUILanguage();
      return getByLangID(lanID);
}

#else

xI18N::xLocaleInfo_t* xI18N::getCurrentLocaleInfor()
{
      return NULL;
}

#endif




END_NAMESPACE_XEVOL3D


