/*$Id: D00-D7F.cc
 *
 * This source file is a part of the Berlin Project
 * Copyright (C) 1999 Tobias Hunger <tobias@berlin-consortium.org>
 * http://www.berlin-consortium.org
 *
 * It was automatically created from the files available at
 * ftp.unicode.org on Wed, 15 Nov 2000 23:24:09 +0100.
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
 
  class Malayalam : public Babylon::Dictionary::Block {
  public:
    void clean () {
    };

    Malayalam() {
      my_first_letter = 0x0D00;
      my_last_letter  = 0x0D7F;
      // my_version="3.0.1" // Not yet supported!
      composeMap[0x0D460D3E] = 0x0D4A;
      composeMap[0x0D460D57] = 0x0D4C;
      composeMap[0x0D470D3E] = 0x0D4B;

    }


    ~Malayalam() {
    }

    _UCS4 firstLetter() {
      return my_first_letter;
    }
 
    _UCS4 lastLetter() {
      return my_last_letter;
    }
 
    // query functions:

    string blockname(const _UCS4 _uc) const {
      return "Malayalam";
    }

    bool isDefined(const _UCS4 _uc) const {
      return (isdefined[_uc - my_first_letter]);
    }

    _UCS4 uppercase(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return _uc;
    }

    _UCS4 lowercase(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return _uc;
    }

    _UCS4 titlecase(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return _uc;
    }

    int decDigitValue(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      switch(_uc) {
      case 0x0D66:
        return 0;
        break;
      case 0x0D67:
        return 1;
        break;
      case 0x0D68:
        return 2;
        break;
      case 0x0D69:
        return 3;
        break;
      case 0x0D6A:
        return 4;
        break;
      case 0x0D6B:
        return 5;
        break;
      case 0x0D6C:
        return 6;
        break;
      case 0x0D6D:
        return 7;
        break;
      case 0x0D6E:
        return 8;
        break;
      case 0x0D6F:
        return 9;
        break;
      default:
        throw UndefinedProperty(_uc, PROP_DEC_DIGIT_VALUE);
      }
    }

    int digitValue(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      switch(_uc) {
      case 0x0D66:
        return 0;
        break;
      case 0x0D67:
        return 1;
        break;
      case 0x0D68:
        return 2;
        break;
      case 0x0D69:
        return 3;
        break;
      case 0x0D6A:
        return 4;
        break;
      case 0x0D6B:
        return 5;
        break;
      case 0x0D6C:
        return 6;
        break;
      case 0x0D6D:
        return 7;
        break;
      case 0x0D6E:
        return 8;
        break;
      case 0x0D6F:
        return 9;
        break;
      default:
        throw UndefinedProperty(_uc, PROP_DIGIT_VALUE);
      }
    }

    float numericValue(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      switch(_uc) {
      case 0x0D66:
        return 0.000000;
        break;
      case 0x0D67:
        return 1.000000;
        break;
      case 0x0D68:
        return 2.000000;
        break;
      case 0x0D69:
        return 3.000000;
        break;
      case 0x0D6A:
        return 4.000000;
        break;
      case 0x0D6B:
        return 5.000000;
        break;
      case 0x0D6C:
        return 6.000000;
        break;
      case 0x0D6D:
        return 7.000000;
        break;
      case 0x0D6E:
        return 8.000000;
        break;
      case 0x0D6F:
        return 9.000000;
        break;
      default:
        throw UndefinedProperty(_uc, PROP_NUMERIC_VALUE);
      }
    }

    Gen_Cat category(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Gen_Cat(Malayalam::cat[_uc - my_first_letter]);
    }

    Can_Comb_Class combClass(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Can_Comb_Class(Malayalam::combCl[_uc - my_first_letter]);
    }

    Bidir_Props bidirProps(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Bidir_Props(Malayalam::bidir[_uc - my_first_letter]);
    }

    Char_Decomp decompType(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Char_Decomp(DECOMP_NO_DECOMP);
    }

    _UTF32String decompose(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      Babylon::_UTF32String us;
      us.resize(2);
      us[0] = Malayalam::decompStr[_uc - my_first_letter][0];
      us[1] = Malayalam::decompStr[_uc - my_first_letter][1];
      if (us[1] == 0x0000) {
        us.resize(1);
      }

      return us;
    }

    bool mustMirror(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return 0;
    }

    Line_Break linebreak(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Line_Break(Malayalam::lb[_uc - my_first_letter]);
    }

    EA_Width EAWidth(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::EA_Width(EA_WIDTH_N);
    }

    _UCS4 compose (const _UCS4 starter, const _UCS4 last) {
      return composeMap[starter << 16 | last];
    }

    bool is_Zero_width(const _UCS4 _uc) const {
      return 0;
    }

    bool is_White_space(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Non_break(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Bidi_Control(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Join_Control(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Format_Control(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Dash(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Hyphen(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Quotation_Mark(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Terminal_Punctuation(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Math(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Paired_Punctuation(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Left_of_Pair(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Combining(const _UCS4 _uc) const {
      return Malayalam::Combining[_uc - my_first_letter];
    }

    bool is_Non_spacing(const _UCS4 _uc) const {
      return Malayalam::Non_spacing[_uc - my_first_letter];
    }

    bool is_Hex_Digit(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Alphabetic(const _UCS4 _uc) const {
      return Malayalam::Alphabetic[_uc - my_first_letter];
    }

    bool is_Diacritic(const _UCS4 _uc) const {
      return Malayalam::Diacritic[_uc - my_first_letter];
    }

    bool is_Extender(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Identifier_Part(const _UCS4 _uc) const {
      return Malayalam::Identifier_Part[_uc - my_first_letter];
    }

    bool is_Ignorable_Control(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Bidi_Hebrew_Right_to_Left(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Bidi_Arabic_Right_to_Left(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Bidi_Embedding_or_Override(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Uppercase(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Lowercase(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Space(const _UCS4 _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Zs);
    }

    bool is_ISO_Control(const _UCS4 _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Cc);
    }

    bool is_Punctuation(const _UCS4 _uc) const {
      return (isDefined(_uc) && (category(_uc) == CAT_Pc ||
                                 category(_uc) == CAT_Pd ||
                                 category(_uc) == CAT_Ps ||
                                 category(_uc) == CAT_Pe ||
                                 category(_uc) == CAT_Pi ||
                                 category(_uc) == CAT_Pf ||
                                 category(_uc) == CAT_Po)
             );
    }

    bool is_Line_Separator(const _UCS4 _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Zl);
    }

    bool is_Paragraph_Separator(const _UCS4 _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Zp);
    }

    bool is_Currency_Symbol(const _UCS4 _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Sc);
    }

    bool is_Titlecase(const _UCS4 _uc) const {
      return (isDefined(_uc) && category(_uc) == CAT_Lt);
    }


  private:
    // functions
    Malayalam(const Malayalam &) {}
 
    Babylon::_UCS4 my_first_letter;
    Babylon::_UCS4 my_last_letter;
    static const bool isdefined[128];
    static const unsigned char cat[128];
    static const unsigned char combCl[128];
    static const unsigned char bidir[128];
    static const _UCS2 decompStr[128][2];
    static const unsigned char lb[128];
    map<_UCS4, _UCS4> composeMap;
    static const bool Combining[128];
    static const bool Non_spacing[128];
    static const bool Alphabetic[128];
    static const bool Diacritic[128];
    static const bool Identifier_Part[128];

  }; // class Malayalam

  const bool Malayalam::isdefined[] = {
    0, 0, 1, 1, 0, 1, 1, 1, 
    1, 1, 1, 1, 1, 0, 1, 1, 
    1, 0, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 0, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 0, 0, 0, 0, 1, 1, 
    1, 1, 1, 1, 0, 0, 1, 1, 
    1, 0, 1, 1, 1, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    1, 1, 0, 0, 0, 0, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0
  };

  const unsigned char Malayalam::cat[] = {
    CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Lo, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Mc, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Mc, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Mc, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
    CAT_Lo, CAT_Lo, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, 
    CAT_Mc, CAT_Mn, CAT_Mn, CAT_Mn, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, 
    CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mn, CAT_Mc, CAT_Mc, 
    CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, 
    CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, 
    CAT_Lo, CAT_Lo, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Nd, CAT_Nd, 
    CAT_Nd, CAT_Nd, CAT_Nd, CAT_Nd, CAT_Nd, CAT_Nd, CAT_Nd, CAT_Nd, 
    CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, 
    CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc, CAT_Mc
  };

  const unsigned char Malayalam::combCl[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 9, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0
  };

  const unsigned char Malayalam::bidir[] = {
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_NSM, BIDIR_NSM, BIDIR_NSM, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_NSM, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L
  };

  const _UCS2 Malayalam::decompStr[][2] = {
    { 0x0D00, 0x0000 }, { 0x0D01, 0x0000 }, { 0x0D02, 0x0000 }, { 0x0D03, 0x0000 }, 
    { 0x0D04, 0x0000 }, { 0x0D05, 0x0000 }, { 0x0D06, 0x0000 }, { 0x0D07, 0x0000 }, 
    { 0x0D08, 0x0000 }, { 0x0D09, 0x0000 }, { 0x0D0A, 0x0000 }, { 0x0D0B, 0x0000 }, 
    { 0x0D0C, 0x0000 }, { 0x0D0D, 0x0000 }, { 0x0D0E, 0x0000 }, { 0x0D0F, 0x0000 }, 
    { 0x0D10, 0x0000 }, { 0x0D11, 0x0000 }, { 0x0D12, 0x0000 }, { 0x0D13, 0x0000 }, 
    { 0x0D14, 0x0000 }, { 0x0D15, 0x0000 }, { 0x0D16, 0x0000 }, { 0x0D17, 0x0000 }, 
    { 0x0D18, 0x0000 }, { 0x0D19, 0x0000 }, { 0x0D1A, 0x0000 }, { 0x0D1B, 0x0000 }, 
    { 0x0D1C, 0x0000 }, { 0x0D1D, 0x0000 }, { 0x0D1E, 0x0000 }, { 0x0D1F, 0x0000 }, 
    { 0x0D20, 0x0000 }, { 0x0D21, 0x0000 }, { 0x0D22, 0x0000 }, { 0x0D23, 0x0000 }, 
    { 0x0D24, 0x0000 }, { 0x0D25, 0x0000 }, { 0x0D26, 0x0000 }, { 0x0D27, 0x0000 }, 
    { 0x0D28, 0x0000 }, { 0x0D29, 0x0000 }, { 0x0D2A, 0x0000 }, { 0x0D2B, 0x0000 }, 
    { 0x0D2C, 0x0000 }, { 0x0D2D, 0x0000 }, { 0x0D2E, 0x0000 }, { 0x0D2F, 0x0000 }, 
    { 0x0D30, 0x0000 }, { 0x0D31, 0x0000 }, { 0x0D32, 0x0000 }, { 0x0D33, 0x0000 }, 
    { 0x0D34, 0x0000 }, { 0x0D35, 0x0000 }, { 0x0D36, 0x0000 }, { 0x0D37, 0x0000 }, 
    { 0x0D38, 0x0000 }, { 0x0D39, 0x0000 }, { 0x0D3A, 0x0000 }, { 0x0D3B, 0x0000 }, 
    { 0x0D3C, 0x0000 }, { 0x0D3D, 0x0000 }, { 0x0D3E, 0x0000 }, { 0x0D3F, 0x0000 }, 
    { 0x0D40, 0x0000 }, { 0x0D41, 0x0000 }, { 0x0D42, 0x0000 }, { 0x0D43, 0x0000 }, 
    { 0x0D44, 0x0000 }, { 0x0D45, 0x0000 }, { 0x0D46, 0x0000 }, { 0x0D47, 0x0000 }, 
    { 0x0D48, 0x0000 }, { 0x0D49, 0x0000 }, { 0x0D46, 0x0D3E }, { 0x0D47, 0x0D3E }, 
    { 0x0D46, 0x0D57 }, { 0x0D4D, 0x0000 }, { 0x0D4E, 0x0000 }, { 0x0D4F, 0x0000 }, 
    { 0x0D50, 0x0000 }, { 0x0D51, 0x0000 }, { 0x0D52, 0x0000 }, { 0x0D53, 0x0000 }, 
    { 0x0D54, 0x0000 }, { 0x0D55, 0x0000 }, { 0x0D56, 0x0000 }, { 0x0D57, 0x0000 }, 
    { 0x0D58, 0x0000 }, { 0x0D59, 0x0000 }, { 0x0D5A, 0x0000 }, { 0x0D5B, 0x0000 }, 
    { 0x0D5C, 0x0000 }, { 0x0D5D, 0x0000 }, { 0x0D5E, 0x0000 }, { 0x0D5F, 0x0000 }, 
    { 0x0D60, 0x0000 }, { 0x0D61, 0x0000 }, { 0x0D62, 0x0000 }, { 0x0D63, 0x0000 }, 
    { 0x0D64, 0x0000 }, { 0x0D65, 0x0000 }, { 0x0D66, 0x0000 }, { 0x0D67, 0x0000 }, 
    { 0x0D68, 0x0000 }, { 0x0D69, 0x0000 }, { 0x0D6A, 0x0000 }, { 0x0D6B, 0x0000 }, 
    { 0x0D6C, 0x0000 }, { 0x0D6D, 0x0000 }, { 0x0D6E, 0x0000 }, { 0x0D6F, 0x0000 }, 
    { 0x0D70, 0x0000 }, { 0x0D71, 0x0000 }, { 0x0D72, 0x0000 }, { 0x0D73, 0x0000 }, 
    { 0x0D74, 0x0000 }, { 0x0D75, 0x0000 }, { 0x0D76, 0x0000 }, { 0x0D77, 0x0000 }, 
    { 0x0D78, 0x0000 }, { 0x0D79, 0x0000 }, { 0x0D7A, 0x0000 }, { 0x0D7B, 0x0000 }, 
    { 0x0D7C, 0x0000 }, { 0x0D7D, 0x0000 }, { 0x0D7E, 0x0000 }, { 0x0D7F, 0x0000 }
  };

  const unsigned char Malayalam::lb[] = {
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_CM, LB_AL, LB_AL, 
    LB_AL, LB_CM, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_CM, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, 
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, 
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, 
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, 
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, 
    LB_AL, LB_AL, LB_CM, LB_CM, LB_CM, LB_CM, LB_NU, LB_NU, 
    LB_NU, LB_NU, LB_NU, LB_NU, LB_NU, LB_NU, LB_NU, LB_NU, 
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, 
    LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM, LB_CM
  };

    const bool Malayalam::Combining[] = {
        0, 0, 1, 1, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 1, 1, 
        1, 1, 1, 1, 0, 0, 1, 1, 
        1, 0, 1, 1, 1, 1, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 1, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool Malayalam::Non_spacing[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 1, 1, 1, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 1, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool Malayalam::Alphabetic[] = {
        0, 0, 1, 1, 0, 1, 1, 1, 
        1, 1, 1, 1, 1, 0, 1, 1, 
        1, 0, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 0, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 0, 0, 0, 0, 1, 1, 
        1, 1, 1, 1, 0, 0, 1, 1, 
        1, 0, 1, 1, 1, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 1, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        1, 1, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool Malayalam::Diacritic[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 1, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool Malayalam::Identifier_Part[] = {
        0, 0, 1, 1, 0, 1, 1, 1, 
        1, 1, 1, 1, 1, 0, 1, 1, 
        1, 0, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 0, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 0, 0, 0, 0, 1, 1, 
        1, 1, 1, 1, 0, 0, 1, 1, 
        1, 0, 1, 1, 1, 1, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 1, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        1, 1, 0, 0, 0, 0, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

}; // namespace Babylon
 
dload(Babylon::Malayalam);