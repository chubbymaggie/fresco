/*$Id: B00-B7F.cc
 *
 * This source file is a part of the Berlin Project
 * Copyright (C) 1999 Tobias Hunger <tobias@berlin-consortium.org>
 * http://www.berlin-consortium.org
 *
 * It was automatically created from the files available at
 * ftp.unicode.org on Tue, 21 Nov 2000 21:11:18 +0100.
 *
 * This plugin to libPrague is free software; you can redistribute it
 * and/or  modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
 * MA 02139, USA
 */

#include <Babylon/defs.hh>
#include <Babylon/Dictionary.hh>
#include <map>

namespace Babylon {
 
  class OriyaB00 : public Babylon::Dictionary::Block {
  public:
    void clean () {
    };

    OriyaB00() {
      my_first_letter = 0xB00;
      my_last_letter  = 0xB7F;
      // my_version="3.0.1" // Not yet supported!
      composeMap[0x0B210B3C] = 0x0B5C;
      composeMap[0x0B220B3C] = 0x0B5D;
      composeMap[0x0B470B3E] = 0x0B4B;
      composeMap[0x0B470B56] = 0x0B48;
      composeMap[0x0B470B57] = 0x0B4C;

    }


    ~OriyaB00() {
    }

    UCS4 firstLetter() {
      return my_first_letter;
    }
 
    UCS4 lastLetter() {
      return my_last_letter;
    }
 
    // query functions:

    string blockname(const UCS4 & _uc) const {
      return "Oriya";
    }

    bool isDefined(const UCS4 & _uc) const {
      return (isdefined[_uc - my_first_letter]);
    }

    UCS4 uppercase(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return _uc;
    }

    UCS4 lowercase(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return _uc;
    }

    UCS4 titlecase(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return _uc;
    }

    int decDigitValue(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      switch(_uc) {
      case 0x0B66:
        return 0;
        break;
      case 0x0B67:
        return 1;
        break;
      case 0x0B68:
        return 2;
        break;
      case 0x0B69:
        return 3;
        break;
      case 0x0B6A:
        return 4;
        break;
      case 0x0B6B:
        return 5;
        break;
      case 0x0B6C:
        return 6;
        break;
      case 0x0B6D:
        return 7;
        break;
      case 0x0B6E:
        return 8;
        break;
      case 0x0B6F:
        return 9;
        break;
      default:
        throw UndefinedProperty(_uc, PROP_DEC_DIGIT_VALUE);
      }
    }

    int digitValue(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      switch(_uc) {
      case 0x0B66:
        return 0;
        break;
      case 0x0B67:
        return 1;
        break;
      case 0x0B68:
        return 2;
        break;
      case 0x0B69:
        return 3;
        break;
      case 0x0B6A:
        return 4;
        break;
      case 0x0B6B:
        return 5;
        break;
      case 0x0B6C:
        return 6;
        break;
      case 0x0B6D:
        return 7;
        break;
      case 0x0B6E:
        return 8;
        break;
      case 0x0B6F:
        return 9;
        break;
      default:
        throw UndefinedProperty(_uc, PROP_DIGIT_VALUE);
      }
    }

    float numericValue(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      switch(_uc) {
      case 0x0B66:
        return 0.000000;
        break;
      case 0x0B67:
        return 1.000000;
        break;
      case 0x0B68:
        return 2.000000;
        break;
      case 0x0B69:
        return 3.000000;
        break;
      case 0x0B6A:
        return 4.000000;
        break;
      case 0x0B6B:
        return 5.000000;
        break;
      case 0x0B6C:
        return 6.000000;
        break;
      case 0x0B6D:
        return 7.000000;
        break;
      case 0x0B6E:
        return 8.000000;
        break;
      case 0x0B6F:
        return 9.000000;
        break;
      default:
        throw UndefinedProperty(_uc, PROP_NUMERIC_VALUE);
      }
    }

    Gen_Cat category(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Gen_Cat(OriyaB00::cat[_uc - my_first_letter]);
    }

    Can_Comb_Class combClass(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Can_Comb_Class(OriyaB00::combCl[_uc - my_first_letter]);
    }

    Bidir_Props bidirProps(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Bidir_Props(OriyaB00::bidir[_uc - my_first_letter]);
    }

    Char_Decomp decompType(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Char_Decomp(DECOMP_NO_DECOMP);
    }

    UTF32String decompose(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      Babylon::UTF32String us;
      us.resize(2);
      us[0] = OriyaB00::decompStr[_uc - my_first_letter][0];
      us[1] = OriyaB00::decompStr[_uc - my_first_letter][1];
      if (us[1] == 0x0000) {
        us.resize(1);
      }

      return us;
    }

    bool mustMirror(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return 0;
    }

    Line_Break linebreak(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Line_Break(OriyaB00::lb[_uc - my_first_letter]);
    }

    EA_Width EAWidth(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::EA_Width(EA_WIDTH_N);
    }

    UCS4 compose (const UCS4 & starter, const UCS4 & last) {
      return composeMap[starter << 16 | last];
    }

    bool is_Zero_width(const UCS4 & _uc) const {
      return 0;
    }

    bool is_White_space(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Non_break(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Bidi_Control(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Join_Control(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Format_Control(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Dash(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Hyphen(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Quotation_Mark(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Terminal_Punctuation(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Math(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Paired_Punctuation(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Left_of_Pair(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Combining(const UCS4 & _uc) const {
      return OriyaB00::Combining[_uc - my_first_letter];
    }

    bool is_Non_spacing(const UCS4 & _uc) const {
      return OriyaB00::Non_spacing[_uc - my_first_letter];
    }

    bool is_Hex_Digit(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Alphabetic(const UCS4 & _uc) const {
      return OriyaB00::Alphabetic[_uc - my_first_letter];
    }

    bool is_Diacritic(const UCS4 & _uc) const {
      return OriyaB00::Diacritic[_uc - my_first_letter];
    }

    bool is_Extender(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Identifier_Part(const UCS4 & _uc) const {
      return OriyaB00::Identifier_Part[_uc - my_first_letter];
    }

    bool is_Ignorable_Control(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Bidi_Hebrew_Right_to_Left(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Bidi_Arabic_Right_to_Left(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Bidi_Embedding_or_Override(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Uppercase(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Lowercase(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Space(const UCS4 & _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Zs);
    }

    bool is_ISO_Control(const UCS4 & _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Cc);
    }

    bool is_Punctuation(const UCS4 & _uc) const {
      return (isDefined(_uc) && (category(_uc) == CAT_Pc ||
                                 category(_uc) == CAT_Pd ||
                                 category(_uc) == CAT_Ps ||
                                 category(_uc) == CAT_Pe ||
                                 category(_uc) == CAT_Pi ||
                                 category(_uc) == CAT_Pf ||
                                 category(_uc) == CAT_Po)
             );
    }

    bool is_Line_Separator(const UCS4 & _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Zl);
    }

    bool is_Paragraph_Separator(const UCS4 & _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Zp);
    }

    bool is_Currency_Symbol(const UCS4 & _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Sc);
    }

    bool is_Titlecase(const UCS4 & _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Lt);
    }


  private:
    // functions
    OriyaB00(const OriyaB00 &) {}
 
    Babylon::UCS4 my_first_letter;
    Babylon::UCS4 my_last_letter;
    static const bool isdefined[128];
    static const unsigned char cat[128];
    static const unsigned char combCl[128];
    static const unsigned char bidir[128];
    static const UCS2 decompStr[128][2];
    static const unsigned char lb[128];
    map<UCS4, UCS4> composeMap;
    static const bool Combining[128];
    static const bool Non_spacing[128];
    static const bool Alphabetic[128];
    static const bool Diacritic[128];
    static const bool Identifier_Part[128];

  }; // class OriyaB00

  const bool OriyaB00::isdefined[] = {
    0, 1, 1, 1, 0, 1, 1, 1, 
    1, 1, 1, 1, 1, 0, 0, 1, 
    1, 0, 0, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 0, 1, 1, 1, 1, 1, 1, 
    1, 0, 1, 1, 0, 0, 1, 1, 
    1, 1, 0, 0, 1, 1, 1, 1, 
    1, 1, 1, 1, 0, 0, 0, 1, 
    1, 0, 0, 1, 1, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 1, 1, 
    0, 0, 0, 0, 1, 1, 0, 1, 
    1, 1, 0, 0, 0, 0, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0
  };

  const unsigned char OriyaB00::cat[] = {
    CAT_Mn, CAT_Mn, CAT_Mc, CAT_Mc, CAT_Mn, CAT_Lo, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Mn, CAT_Mn, CAT_Lo, 
    CAT_Lo, CAT_Mn, CAT_Mn, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Mn, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Mn, CAT_Lo, CAT_Lo, CAT_Mn, CAT_Mn, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Lo, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Lo, CAT_Mc, CAT_Mn, 
    CAT_Mc, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mc, 
    CAT_Mc, CAT_Mn, CAT_Mn, CAT_Mc, CAT_Mc, CAT_Mn, CAT_Mn, CAT_Mn, 
    CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mc, 
    CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Lo, CAT_Lo, CAT_Mn, CAT_Lo, 
    CAT_Lo, CAT_Lo, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Nd, CAT_Nd, 
    CAT_Nd, CAT_Nd, CAT_Nd, CAT_Nd, CAT_Nd, CAT_Nd, CAT_Nd, CAT_Nd, 
    CAT_So, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, 
    CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mn
  };

  const unsigned char OriyaB00::combCl[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 7, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 9, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0
  };

  const unsigned char OriyaB00::bidir[] = {
    BIDIR_NSM, BIDIR_NSM, BIDIR_L, BIDIR_L, BIDIR_NSM, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_NSM, BIDIR_NSM, BIDIR_L, 
    BIDIR_L, BIDIR_NSM, BIDIR_NSM, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_NSM, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_NSM, BIDIR_L, BIDIR_L, BIDIR_NSM, BIDIR_NSM, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_L, BIDIR_L, BIDIR_NSM, 
    BIDIR_L, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_L, 
    BIDIR_L, BIDIR_NSM, BIDIR_NSM, BIDIR_L, BIDIR_L, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, 
    BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_L, 
    BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_L, BIDIR_L, BIDIR_NSM, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, 
    BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM
  };

  const UCS2 OriyaB00::decompStr[][2] = {
    { 0x0B00, 0x0000 }, { 0x0B01, 0x0000 }, { 0x0B02, 0x0000 }, { 0x0B03, 0x0000 }, 
    { 0x0B04, 0x0000 }, { 0x0B05, 0x0000 }, { 0x0B06, 0x0000 }, { 0x0B07, 0x0000 }, 
    { 0x0B08, 0x0000 }, { 0x0B09, 0x0000 }, { 0x0B0A, 0x0000 }, { 0x0B0B, 0x0000 }, 
    { 0x0B0C, 0x0000 }, { 0x0B0D, 0x0000 }, { 0x0B0E, 0x0000 }, { 0x0B0F, 0x0000 }, 
    { 0x0B10, 0x0000 }, { 0x0B11, 0x0000 }, { 0x0B12, 0x0000 }, { 0x0B13, 0x0000 }, 
    { 0x0B14, 0x0000 }, { 0x0B15, 0x0000 }, { 0x0B16, 0x0000 }, { 0x0B17, 0x0000 }, 
    { 0x0B18, 0x0000 }, { 0x0B19, 0x0000 }, { 0x0B1A, 0x0000 }, { 0x0B1B, 0x0000 }, 
    { 0x0B1C, 0x0000 }, { 0x0B1D, 0x0000 }, { 0x0B1E, 0x0000 }, { 0x0B1F, 0x0000 }, 
    { 0x0B20, 0x0000 }, { 0x0B21, 0x0000 }, { 0x0B22, 0x0000 }, { 0x0B23, 0x0000 }, 
    { 0x0B24, 0x0000 }, { 0x0B25, 0x0000 }, { 0x0B26, 0x0000 }, { 0x0B27, 0x0000 }, 
    { 0x0B28, 0x0000 }, { 0x0B29, 0x0000 }, { 0x0B2A, 0x0000 }, { 0x0B2B, 0x0000 }, 
    { 0x0B2C, 0x0000 }, { 0x0B2D, 0x0000 }, { 0x0B2E, 0x0000 }, { 0x0B2F, 0x0000 }, 
    { 0x0B30, 0x0000 }, { 0x0B31, 0x0000 }, { 0x0B32, 0x0000 }, { 0x0B33, 0x0000 }, 
    { 0x0B34, 0x0000 }, { 0x0B35, 0x0000 }, { 0x0B36, 0x0000 }, { 0x0B37, 0x0000 }, 
    { 0x0B38, 0x0000 }, { 0x0B39, 0x0000 }, { 0x0B3A, 0x0000 }, { 0x0B3B, 0x0000 }, 
    { 0x0B3C, 0x0000 }, { 0x0B3D, 0x0000 }, { 0x0B3E, 0x0000 }, { 0x0B3F, 0x0000 }, 
    { 0x0B40, 0x0000 }, { 0x0B41, 0x0000 }, { 0x0B42, 0x0000 }, { 0x0B43, 0x0000 }, 
    { 0x0B44, 0x0000 }, { 0x0B45, 0x0000 }, { 0x0B46, 0x0000 }, { 0x0B47, 0x0000 }, 
    { 0x0B47, 0x0B56 }, { 0x0B49, 0x0000 }, { 0x0B4A, 0x0000 }, { 0x0B47, 0x0B3E }, 
    { 0x0B47, 0x0B57 }, { 0x0B4D, 0x0000 }, { 0x0B4E, 0x0000 }, { 0x0B4F, 0x0000 }, 
    { 0x0B50, 0x0000 }, { 0x0B51, 0x0000 }, { 0x0B52, 0x0000 }, { 0x0B53, 0x0000 }, 
    { 0x0B54, 0x0000 }, { 0x0B55, 0x0000 }, { 0x0B56, 0x0000 }, { 0x0B57, 0x0000 }, 
    { 0x0B58, 0x0000 }, { 0x0B59, 0x0000 }, { 0x0B5A, 0x0000 }, { 0x0B5B, 0x0000 }, 
    { 0x0B21, 0x0B3C }, { 0x0B22, 0x0B3C }, { 0x0B5E, 0x0000 }, { 0x0B5F, 0x0000 }, 
    { 0x0B60, 0x0000 }, { 0x0B61, 0x0000 }, { 0x0B62, 0x0000 }, { 0x0B63, 0x0000 }, 
    { 0x0B64, 0x0000 }, { 0x0B65, 0x0000 }, { 0x0B66, 0x0000 }, { 0x0B67, 0x0000 }, 
    { 0x0B68, 0x0000 }, { 0x0B69, 0x0000 }, { 0x0B6A, 0x0000 }, { 0x0B6B, 0x0000 }, 
    { 0x0B6C, 0x0000 }, { 0x0B6D, 0x0000 }, { 0x0B6E, 0x0000 }, { 0x0B6F, 0x0000 }, 
    { 0x0B70, 0x0000 }, { 0x0B71, 0x0000 }, { 0x0B72, 0x0000 }, { 0x0B73, 0x0000 }, 
    { 0x0B74, 0x0000 }, { 0x0B75, 0x0000 }, { 0x0B76, 0x0000 }, { 0x0B77, 0x0000 }, 
    { 0x0B78, 0x0000 }, { 0x0B79, 0x0000 }, { 0x0B7A, 0x0000 }, { 0x0B7B, 0x0000 }, 
    { 0x0B7C, 0x0000 }, { 0x0B7D, 0x0000 }, { 0x0B7E, 0x0000 }, { 0x0B7F, 0x0000 }
  };

  const unsigned char OriyaB00::lb[] = {
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_CM, LB_CM, LB_AL, 
    LB_AL, LB_CM, LB_CM, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_CM, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_CM, LB_AL, LB_AL, LB_CM, LB_CM, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_CM, LB_CM, LB_CM, LB_AL, LB_CM, LB_CM, 
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, 
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, 
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, 
    LB_CM, LB_CM, LB_CM, LB_CM, LB_AL, LB_AL, LB_CM, LB_AL, 
    LB_AL, LB_AL, LB_CM, LB_CM, LB_CM, LB_CM, LB_NU, LB_NU, 
    LB_NU, LB_NU, LB_NU, LB_NU, LB_NU, LB_NU, LB_NU, LB_NU, 
    LB_AL, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, 
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM
  };

    const bool OriyaB00::Combining[] = {
        0, 1, 1, 1, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 1, 0, 1, 1, 
        1, 1, 1, 1, 0, 0, 0, 1, 
        1, 0, 0, 1, 1, 1, 0, 0, 
        0, 0, 0, 0, 0, 0, 1, 1, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool OriyaB00::Non_spacing[] = {
        0, 1, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 1, 0, 0, 1, 
        0, 1, 1, 1, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 1, 0, 0, 
        0, 0, 0, 0, 0, 0, 1, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool OriyaB00::Alphabetic[] = {
        0, 1, 1, 1, 0, 1, 1, 1, 
        1, 1, 1, 1, 1, 0, 0, 1, 
        1, 0, 0, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 0, 1, 1, 1, 1, 1, 1, 
        1, 0, 1, 1, 0, 0, 1, 1, 
        1, 1, 0, 0, 0, 1, 1, 1, 
        1, 1, 1, 1, 0, 0, 0, 1, 
        1, 0, 0, 1, 1, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 1, 1, 
        0, 0, 0, 0, 1, 1, 0, 1, 
        1, 1, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool OriyaB00::Diacritic[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 1, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 1, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool OriyaB00::Identifier_Part[] = {
        0, 1, 1, 1, 0, 1, 1, 1, 
        1, 1, 1, 1, 1, 0, 0, 1, 
        1, 0, 0, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 0, 1, 1, 1, 1, 1, 1, 
        1, 0, 1, 1, 0, 0, 1, 1, 
        1, 1, 0, 0, 1, 1, 1, 1, 
        1, 1, 1, 1, 0, 0, 0, 1, 
        1, 0, 0, 1, 1, 1, 0, 0, 
        0, 0, 0, 0, 0, 0, 1, 1, 
        0, 0, 0, 0, 1, 1, 0, 1, 
        1, 1, 0, 0, 0, 0, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

}; // namespace Babylon
 
dload(Babylon::OriyaB00);
