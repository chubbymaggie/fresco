/*$Id: 530-58F.cc
 *
 * This source file is a part of the Berlin Project
 * Copyright (C) 1999 Tobias Hunger <tobias@berlin-consortium.org>
 * http://www.berlin-consortium.org
 *
 * It was automatically created from the files available at
 * ftp.unicode.org on Wed, 15 Nov 2000 23:23:03 +0100.
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
 
  class Armenian : public Babylon::Dictionary::Block {
  public:
    void clean () {
    };

    Armenian() {
      my_first_letter = 0x0530;
      my_last_letter  = 0x058F;
      // my_version="3.0.1" // Not yet supported!

    }


    ~Armenian() {
    }

    _UCS4 firstLetter() {
      return my_first_letter;
    }
 
    _UCS4 lastLetter() {
      return my_last_letter;
    }
 
    // query functions:

    string blockname(const _UCS4 _uc) const {
      return "Armenian";
    }

    bool isDefined(const _UCS4 _uc) const {
      return (isdefined[_uc - my_first_letter]);
    }

    _UCS4 uppercase(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Armenian::upper[_uc - my_first_letter];
    }

    _UCS4 lowercase(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Armenian::lower[_uc - my_first_letter];
    }

    _UCS4 titlecase(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Armenian::title[_uc - my_first_letter];
    }

    int decDigitValue(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      throw UndefinedProperty(_uc, PROP_DEC_DIGIT_VALUE);
    }

    int digitValue(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      throw UndefinedProperty(_uc, PROP_DIGIT_VALUE);
    }

    float numericValue(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      throw UndefinedProperty(_uc, PROP_NUMERIC_VALUE);
    }

    Gen_Cat category(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Gen_Cat(Armenian::cat[_uc - my_first_letter]);
    }

    Can_Comb_Class combClass(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Can_Comb_Class(0);
    }

    Bidir_Props bidirProps(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Bidir_Props(Armenian::bidir[_uc - my_first_letter]);
    }

    Char_Decomp decompType(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::Char_Decomp(Armenian::decomp[_uc - my_first_letter]);
    }

    _UTF32String decompose(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      Babylon::_UTF32String us;
      us.resize(2);
      us[0] = Armenian::decompStr[_uc - my_first_letter][0];
      us[1] = Armenian::decompStr[_uc - my_first_letter][1];
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
      return Babylon::Line_Break(Armenian::lb[_uc - my_first_letter]);
    }

    EA_Width EAWidth(const _UCS4 _uc) const {
      if (!isDefined(_uc))
        throw UndefinedProperty(_uc, PROP_CHARACTER);
      return Babylon::EA_Width(EA_WIDTH_N);
    }

    _UCS4 compose (const _UCS4 starter, const _UCS4 last) {
      return 0;
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
      return Armenian::Dash[_uc - my_first_letter];
    }

    bool is_Hyphen(const _UCS4 _uc) const {
      return Armenian::Hyphen[_uc - my_first_letter];
    }

    bool is_Quotation_Mark(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Terminal_Punctuation(const _UCS4 _uc) const {
      return Armenian::Terminal_Punctuation[_uc - my_first_letter];
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
      return 0;
    }

    bool is_Non_spacing(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Hex_Digit(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Alphabetic(const _UCS4 _uc) const {
      return Armenian::Alphabetic[_uc - my_first_letter];
    }

    bool is_Diacritic(const _UCS4 _uc) const {
      return Armenian::Diacritic[_uc - my_first_letter];
    }

    bool is_Extender(const _UCS4 _uc) const {
      return 0;
    }

    bool is_Identifier_Part(const _UCS4 _uc) const {
      return Armenian::Identifier_Part[_uc - my_first_letter];
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
      return Armenian::Uppercase[_uc - my_first_letter];
    }

    bool is_Lowercase(const _UCS4 _uc) const {
      return Armenian::Lowercase[_uc - my_first_letter];
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
    Armenian(const Armenian &) {}
 
    Babylon::_UCS4 my_first_letter;
    Babylon::_UCS4 my_last_letter;
    static const bool isdefined[96];
    static const _UCS4 upper[96];
    static const _UCS4 lower[96];
    static const _UCS4 title[96];
    static const unsigned char cat[96];
    static const unsigned char bidir[96];
    static const unsigned char decomp[96];
    static const _UCS2 decompStr[96][2];
    static const unsigned char lb[96];
    static const bool Dash[96];
    static const bool Hyphen[96];
    static const bool Terminal_Punctuation[96];
    static const bool Alphabetic[96];
    static const bool Diacritic[96];
    static const bool Identifier_Part[96];
    static const bool Uppercase[96];
    static const bool Lowercase[96];

  }; // class Armenian

  const bool Armenian::isdefined[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 0, 
    0, 1, 1, 1, 1, 1, 1, 1, 
    0, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    0, 1, 1, 0, 0, 0, 0, 0
  };

  const _UCS4 Armenian::upper[] = {
    0x0530, 0x0531, 0x0532, 0x0533, 0x0534, 0x0535, 0x0536, 0x0537, 
    0x0538, 0x0539, 0x053A, 0x053B, 0x053C, 0x053D, 0x053E, 0x053F, 
    0x0540, 0x0541, 0x0542, 0x0543, 0x0544, 0x0545, 0x0546, 0x0547, 
    0x0548, 0x0549, 0x054A, 0x054B, 0x054C, 0x054D, 0x054E, 0x054F, 
    0x0550, 0x0551, 0x0552, 0x0553, 0x0554, 0x0555, 0x0556, 0x0557, 
    0x0558, 0x0559, 0x055A, 0x055B, 0x055C, 0x055D, 0x055E, 0x055F, 
    0x0560, 0x0531, 0x0532, 0x0533, 0x0534, 0x0535, 0x0536, 0x0537, 
    0x0538, 0x0539, 0x053A, 0x053B, 0x053C, 0x053D, 0x053E, 0x053F, 
    0x0540, 0x0541, 0x0542, 0x0543, 0x0544, 0x0545, 0x0546, 0x0547, 
    0x0548, 0x0549, 0x054A, 0x054B, 0x054C, 0x054D, 0x054E, 0x054F, 
    0x0550, 0x0551, 0x0552, 0x0553, 0x0554, 0x0555, 0x0556, 0x0587, 
    0x0588, 0x0589, 0x058A, 0x058B, 0x058C, 0x058D, 0x058E, 0x058F
  };

  const _UCS4 Armenian::lower[] = {
    0x0530, 0x0561, 0x0562, 0x0563, 0x0564, 0x0565, 0x0566, 0x0567, 
    0x0568, 0x0569, 0x056A, 0x056B, 0x056C, 0x056D, 0x056E, 0x056F, 
    0x0570, 0x0571, 0x0572, 0x0573, 0x0574, 0x0575, 0x0576, 0x0577, 
    0x0578, 0x0579, 0x057A, 0x057B, 0x057C, 0x057D, 0x057E, 0x057F, 
    0x0580, 0x0581, 0x0582, 0x0583, 0x0584, 0x0585, 0x0586, 0x0557, 
    0x0558, 0x0559, 0x055A, 0x055B, 0x055C, 0x055D, 0x055E, 0x055F, 
    0x0560, 0x0561, 0x0562, 0x0563, 0x0564, 0x0565, 0x0566, 0x0567, 
    0x0568, 0x0569, 0x056A, 0x056B, 0x056C, 0x056D, 0x056E, 0x056F, 
    0x0570, 0x0571, 0x0572, 0x0573, 0x0574, 0x0575, 0x0576, 0x0577, 
    0x0578, 0x0579, 0x057A, 0x057B, 0x057C, 0x057D, 0x057E, 0x057F, 
    0x0580, 0x0581, 0x0582, 0x0583, 0x0584, 0x0585, 0x0586, 0x0587, 
    0x0588, 0x0589, 0x058A, 0x058B, 0x058C, 0x058D, 0x058E, 0x058F
  };

  const _UCS4 Armenian::title[] = {
    0x0530, 0x0531, 0x0532, 0x0533, 0x0534, 0x0535, 0x0536, 0x0537, 
    0x0538, 0x0539, 0x053A, 0x053B, 0x053C, 0x053D, 0x053E, 0x053F, 
    0x0540, 0x0541, 0x0542, 0x0543, 0x0544, 0x0545, 0x0546, 0x0547, 
    0x0548, 0x0549, 0x054A, 0x054B, 0x054C, 0x054D, 0x054E, 0x054F, 
    0x0550, 0x0551, 0x0552, 0x0553, 0x0554, 0x0555, 0x0556, 0x0557, 
    0x0558, 0x0559, 0x055A, 0x055B, 0x055C, 0x055D, 0x055E, 0x055F, 
    0x0560, 0x0531, 0x0532, 0x0533, 0x0534, 0x0535, 0x0536, 0x0537, 
    0x0538, 0x0539, 0x053A, 0x053B, 0x053C, 0x053D, 0x053E, 0x053F, 
    0x0540, 0x0541, 0x0542, 0x0543, 0x0544, 0x0545, 0x0546, 0x0547, 
    0x0548, 0x0549, 0x054A, 0x054B, 0x054C, 0x054D, 0x054E, 0x054F, 
    0x0550, 0x0551, 0x0552, 0x0553, 0x0554, 0x0555, 0x0556, 0x0587, 
    0x0588, 0x0589, 0x058A, 0x058B, 0x058C, 0x058D, 0x058E, 0x058F
  };

  const unsigned char Armenian::cat[] = {
    CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, 
    CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, 
    CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, 
    CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, 
    CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, 
    CAT_Lu, CAT_Lm, CAT_Po, CAT_Po, CAT_Po, CAT_Po, CAT_Po, CAT_Po, 
    CAT_Lu, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, 
    CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, 
    CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, 
    CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, 
    CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, CAT_Ll, 
    CAT_Lu, CAT_Po, CAT_Pd, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu, CAT_Lu
  };

  const unsigned char Armenian::bidir[] = {
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, 
    BIDIR_L, BIDIR_L, BIDIR_ON, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L, BIDIR_L
  };

  const unsigned char Armenian::decomp[] = {
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
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_COMPAT, 
    DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP, DECOMP_NO_DECOMP
  };

  const _UCS2 Armenian::decompStr[][2] = {
    { 0x0530, 0x0000 }, { 0x0531, 0x0000 }, { 0x0532, 0x0000 }, { 0x0533, 0x0000 }, 
    { 0x0534, 0x0000 }, { 0x0535, 0x0000 }, { 0x0536, 0x0000 }, { 0x0537, 0x0000 }, 
    { 0x0538, 0x0000 }, { 0x0539, 0x0000 }, { 0x053A, 0x0000 }, { 0x053B, 0x0000 }, 
    { 0x053C, 0x0000 }, { 0x053D, 0x0000 }, { 0x053E, 0x0000 }, { 0x053F, 0x0000 }, 
    { 0x0540, 0x0000 }, { 0x0541, 0x0000 }, { 0x0542, 0x0000 }, { 0x0543, 0x0000 }, 
    { 0x0544, 0x0000 }, { 0x0545, 0x0000 }, { 0x0546, 0x0000 }, { 0x0547, 0x0000 }, 
    { 0x0548, 0x0000 }, { 0x0549, 0x0000 }, { 0x054A, 0x0000 }, { 0x054B, 0x0000 }, 
    { 0x054C, 0x0000 }, { 0x054D, 0x0000 }, { 0x054E, 0x0000 }, { 0x054F, 0x0000 }, 
    { 0x0550, 0x0000 }, { 0x0551, 0x0000 }, { 0x0552, 0x0000 }, { 0x0553, 0x0000 }, 
    { 0x0554, 0x0000 }, { 0x0555, 0x0000 }, { 0x0556, 0x0000 }, { 0x0557, 0x0000 }, 
    { 0x0558, 0x0000 }, { 0x0559, 0x0000 }, { 0x055A, 0x0000 }, { 0x055B, 0x0000 }, 
    { 0x055C, 0x0000 }, { 0x055D, 0x0000 }, { 0x055E, 0x0000 }, { 0x055F, 0x0000 }, 
    { 0x0560, 0x0000 }, { 0x0561, 0x0000 }, { 0x0562, 0x0000 }, { 0x0563, 0x0000 }, 
    { 0x0564, 0x0000 }, { 0x0565, 0x0000 }, { 0x0566, 0x0000 }, { 0x0567, 0x0000 }, 
    { 0x0568, 0x0000 }, { 0x0569, 0x0000 }, { 0x056A, 0x0000 }, { 0x056B, 0x0000 }, 
    { 0x056C, 0x0000 }, { 0x056D, 0x0000 }, { 0x056E, 0x0000 }, { 0x056F, 0x0000 }, 
    { 0x0570, 0x0000 }, { 0x0571, 0x0000 }, { 0x0572, 0x0000 }, { 0x0573, 0x0000 }, 
    { 0x0574, 0x0000 }, { 0x0575, 0x0000 }, { 0x0576, 0x0000 }, { 0x0577, 0x0000 }, 
    { 0x0578, 0x0000 }, { 0x0579, 0x0000 }, { 0x057A, 0x0000 }, { 0x057B, 0x0000 }, 
    { 0x057C, 0x0000 }, { 0x057D, 0x0000 }, { 0x057E, 0x0000 }, { 0x057F, 0x0000 }, 
    { 0x0580, 0x0000 }, { 0x0581, 0x0000 }, { 0x0582, 0x0000 }, { 0x0583, 0x0000 }, 
    { 0x0584, 0x0000 }, { 0x0585, 0x0000 }, { 0x0586, 0x0000 }, { 0x0565, 0x0582 }, 
    { 0x0588, 0x0000 }, { 0x0589, 0x0000 }, { 0x058A, 0x0000 }, { 0x058B, 0x0000 }, 
    { 0x058C, 0x0000 }, { 0x058D, 0x0000 }, { 0x058E, 0x0000 }, { 0x058F, 0x0000 }
  };

  const unsigned char Armenian::lb[] = {
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL, 
    LB_AL, LB_IS, LB_BA, LB_AL, LB_AL, LB_AL, LB_AL, LB_AL
  };

    const bool Armenian::Dash[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 1, 0, 0, 0, 0, 0
    };

    const bool Armenian::Hyphen[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 1, 0, 0, 0, 0, 0
    };

    const bool Armenian::Terminal_Punctuation[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 1, 0, 0, 0, 0, 0, 0
    };

    const bool Armenian::Alphabetic[] = {
        0, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 0, 
        0, 1, 0, 0, 0, 0, 0, 0, 
        0, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool Armenian::Diacritic[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 1, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool Armenian::Identifier_Part[] = {
        0, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 0, 
        0, 1, 0, 0, 0, 0, 0, 0, 
        0, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool Armenian::Uppercase[] = {
        0, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const bool Armenian::Lowercase[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
        0, 0, 0, 0, 0, 0, 0, 0
    };

}; // namespace Babylon
 
dload(Babylon::Armenian);