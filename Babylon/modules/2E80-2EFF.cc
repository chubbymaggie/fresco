/*$Id: 2E80-2EFF.cc
 *
 * This source file is a part of the Berlin Project
 * Copyright (C) 1999 Tobias Hunger <tobias@berlin-consortium.org>
 * http://www.berlin-consortium.org
 *
 * It was automatically created from the files available at
 * ftp.unicode.org on Tue, 21 Nov 2000 21:13:05 +0100.
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

namespace Babylon {
 
  class CJK_Radicals_Supplement2E80 : public Babylon::Dictionary::Block {
  public:
    void clean () {
    };

    CJK_Radicals_Supplement2E80() {
      my_first_letter = 0x2E80;
      my_last_letter  = 0x2EFF;
      // my_version="3.0.1" // Not yet supported!

    }


    ~CJK_Radicals_Supplement2E80() {
    }

    UCS4 firstLetter() {
      return my_first_letter;
    }
 
    UCS4 lastLetter() {
      return my_last_letter;
    }
 
    // query functions:

    string blockname(const UCS4 & _uc) const {
      return "CJK Radicals Supplement";
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
      throw UndefinedProperty(_uc, PROP_DEC_DIGIT_VALUE);
    }

    int digitValue(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      throw UndefinedProperty(_uc, PROP_DIGIT_VALUE);
    }

    float numericValue(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      throw UndefinedProperty(_uc, PROP_NUMERIC_VALUE);
    }

    Gen_Cat category(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Gen_Cat(CAT_So);
    }

    Can_Comb_Class combClass(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Can_Comb_Class(0);
    }

    Bidir_Props bidirProps(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Bidir_Props(BIDIR_ON);
    }

    Char_Decomp decompType(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Char_Decomp(CJK_Radicals_Supplement2E80::decomp[_uc - my_first_letter]);
    }

    UTF32String decompose(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      Babylon::UTF32String us;
      us.resize(2);
      us[0] = CJK_Radicals_Supplement2E80::decompStr[_uc - my_first_letter][0];
      us[1] = CJK_Radicals_Supplement2E80::decompStr[_uc - my_first_letter][1];
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
      return Babylon::Line_Break(LB_ID);
    }

    EA_Width EAWidth(const UCS4 & _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::EA_Width(EA_WIDTH_W);
    }

    UCS4 compose (const UCS4 & starter, const UCS4 & last) {
      return 0;
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
      return 0;
    }

    bool is_Non_spacing(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Hex_Digit(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Alphabetic(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Diacritic(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Extender(const UCS4 & _uc) const {
      return 0;
    }

    bool is_Identifier_Part(const UCS4 & _uc) const {
      return 0;
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
    CJK_Radicals_Supplement2E80(const CJK_Radicals_Supplement2E80 &) {}
 
    Babylon::UCS4 my_first_letter;
    Babylon::UCS4 my_last_letter;
    static const bool isdefined[128];
    static const unsigned char decomp[128];
    static const UCS2 decompStr[128][2];

  }; // class CJK_Radicals_Supplement2E80

  const bool CJK_Radicals_Supplement2E80::isdefined[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 0, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0
  };

  const unsigned char CJK_Radicals_Supplement2E80::decomp[] = {
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_COMPAT, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_COMPAT, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP
  };

  const UCS2 CJK_Radicals_Supplement2E80::decompStr[][2] = {
    { 0x2E80, 0x0000 }, { 0x2E81, 0x0000 }, { 0x2E82, 0x0000 }, { 0x2E83, 0x0000 }, 
    { 0x2E84, 0x0000 }, { 0x2E85, 0x0000 }, { 0x2E86, 0x0000 }, { 0x2E87, 0x0000 }, 
    { 0x2E88, 0x0000 }, { 0x2E89, 0x0000 }, { 0x2E8A, 0x0000 }, { 0x2E8B, 0x0000 }, 
    { 0x2E8C, 0x0000 }, { 0x2E8D, 0x0000 }, { 0x2E8E, 0x0000 }, { 0x2E8F, 0x0000 }, 
    { 0x2E90, 0x0000 }, { 0x2E91, 0x0000 }, { 0x2E92, 0x0000 }, { 0x2E93, 0x0000 }, 
    { 0x2E94, 0x0000 }, { 0x2E95, 0x0000 }, { 0x2E96, 0x0000 }, { 0x2E97, 0x0000 }, 
    { 0x2E98, 0x0000 }, { 0x2E99, 0x0000 }, { 0x2E9A, 0x0000 }, { 0x2E9B, 0x0000 }, 
    { 0x2E9C, 0x0000 }, { 0x2E9D, 0x0000 }, { 0x2E9E, 0x0000 }, { 0x6BCD, 0x0000 }, 
    { 0x2EA0, 0x0000 }, { 0x2EA1, 0x0000 }, { 0x2EA2, 0x0000 }, { 0x2EA3, 0x0000 }, 
    { 0x2EA4, 0x0000 }, { 0x2EA5, 0x0000 }, { 0x2EA6, 0x0000 }, { 0x2EA7, 0x0000 }, 
    { 0x2EA8, 0x0000 }, { 0x2EA9, 0x0000 }, { 0x2EAA, 0x0000 }, { 0x2EAB, 0x0000 }, 
    { 0x2EAC, 0x0000 }, { 0x2EAD, 0x0000 }, { 0x2EAE, 0x0000 }, { 0x2EAF, 0x0000 }, 
    { 0x2EB0, 0x0000 }, { 0x2EB1, 0x0000 }, { 0x2EB2, 0x0000 }, { 0x2EB3, 0x0000 }, 
    { 0x2EB4, 0x0000 }, { 0x2EB5, 0x0000 }, { 0x2EB6, 0x0000 }, { 0x2EB7, 0x0000 }, 
    { 0x2EB8, 0x0000 }, { 0x2EB9, 0x0000 }, { 0x2EBA, 0x0000 }, { 0x2EBB, 0x0000 }, 
    { 0x2EBC, 0x0000 }, { 0x2EBD, 0x0000 }, { 0x2EBE, 0x0000 }, { 0x2EBF, 0x0000 }, 
    { 0x2EC0, 0x0000 }, { 0x2EC1, 0x0000 }, { 0x2EC2, 0x0000 }, { 0x2EC3, 0x0000 }, 
    { 0x2EC4, 0x0000 }, { 0x2EC5, 0x0000 }, { 0x2EC6, 0x0000 }, { 0x2EC7, 0x0000 }, 
    { 0x2EC8, 0x0000 }, { 0x2EC9, 0x0000 }, { 0x2ECA, 0x0000 }, { 0x2ECB, 0x0000 }, 
    { 0x2ECC, 0x0000 }, { 0x2ECD, 0x0000 }, { 0x2ECE, 0x0000 }, { 0x2ECF, 0x0000 }, 
    { 0x2ED0, 0x0000 }, { 0x2ED1, 0x0000 }, { 0x2ED2, 0x0000 }, { 0x2ED3, 0x0000 }, 
    { 0x2ED4, 0x0000 }, { 0x2ED5, 0x0000 }, { 0x2ED6, 0x0000 }, { 0x2ED7, 0x0000 }, 
    { 0x2ED8, 0x0000 }, { 0x2ED9, 0x0000 }, { 0x2EDA, 0x0000 }, { 0x2EDB, 0x0000 }, 
    { 0x2EDC, 0x0000 }, { 0x2EDD, 0x0000 }, { 0x2EDE, 0x0000 }, { 0x2EDF, 0x0000 }, 
    { 0x2EE0, 0x0000 }, { 0x2EE1, 0x0000 }, { 0x2EE2, 0x0000 }, { 0x2EE3, 0x0000 }, 
    { 0x2EE4, 0x0000 }, { 0x2EE5, 0x0000 }, { 0x2EE6, 0x0000 }, { 0x2EE7, 0x0000 }, 
    { 0x2EE8, 0x0000 }, { 0x2EE9, 0x0000 }, { 0x2EEA, 0x0000 }, { 0x2EEB, 0x0000 }, 
    { 0x2EEC, 0x0000 }, { 0x2EED, 0x0000 }, { 0x2EEE, 0x0000 }, { 0x2EEF, 0x0000 }, 
    { 0x2EF0, 0x0000 }, { 0x2EF1, 0x0000 }, { 0x2EF2, 0x0000 }, { 0x9F9F, 0x0000 }, 
    { 0x2EF4, 0x0000 }, { 0x2EF5, 0x0000 }, { 0x2EF6, 0x0000 }, { 0x2EF7, 0x0000 }, 
    { 0x2EF8, 0x0000 }, { 0x2EF9, 0x0000 }, { 0x2EFA, 0x0000 }, { 0x2EFB, 0x0000 }, 
    { 0x2EFC, 0x0000 }, { 0x2EFD, 0x0000 }, { 0x2EFE, 0x0000 }, { 0x2EFF, 0x0000 }
  };

}; // namespace Babylon
 
dload(Babylon::CJK_Radicals_Supplement2E80);
