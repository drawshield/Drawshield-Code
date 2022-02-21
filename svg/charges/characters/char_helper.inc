<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

     DrawShield is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */

// word

$charge['default_colour'] = 'sable';

foreach ($node->childNodes as $child) {
    if ($child->nodeName == blazonML::E_MODIFIER) {
        switch ($child->getAttribute(blazonML::A_KEYTERM)) {
            case 'opposite':
                $item = $child->getAttribute(blazonML::A_PARAM);
                if ( $item == null ) $item = 'huh?';
                $builder->setOption(WordBuilder::TEXT2, $item);
                break;
            case 'bold':
            case 'italic':
                $messages->addMessage('blazon','Bold and italic are now longer supported');
                break;
            case 'font':
                $builder->setOption(WordBuilder::FONT, getModifierValueByKeyterm($node, 'font'));
                break;
            case 'engraved':
            case 'cursive':
            case 'gothic':
            case 'serif':
            case 'plain':
            case 'carved':
                $builder->setOption(WordBuilder::FONT, $child->getAttribute(blazonML::A_KEYTERM));
                break;
            case 'underlined':
                $builder->setOption(WordBuilder::UNDERLINE);
                break;
            case  'to-dexter':
                $builder->setOption(WordBuilder::ALIGN, WordBuilder::LEFT);
                break;
            case  'to-sinister':
                $builder->setOption(WordBuilder::ALIGN, WordBuilder::RIGHT);
                break;
            case  'to-centre':
                $builder->setOption(WordBuilder::ALIGN, WordBuilder::CENTRE);
                break;
            case 'encircled':
                $builder->setOption(WordBuilder::LAYOUT, WordBuilder::ENCIRCLED);
                break;
            case 'text-inline':
                $builder->setOption(WordBuilder::LAYOUT, WordBuilder::INLINE);
                break;
            case 'text-stacked':
                $builder->setOption(WordBuilder::LAYOUT, WordBuilder::STACKED);
                break;
            case 'text-block':
                $messages->addMessage('blazon','Text in blocks is no longer supported');
                // $builder->setOption(WordBuilder::LAYOUT, WordBuilder::BLOCK);
                break;
            case 'enarched':
                $builder->setOption(WordBuilder::LAYOUT, WordBuilder::ENARCHED);
                break;
            case 'embowed':
                $builder->setOption(WordBuilder::LAYOUT, WordBuilder::EMBOWED);
                break;
            case 'wavy':
                $builder->setOption(WordBuilder::LAYOUT, WordBuilder::WAVY);
                break;

        }
    }
}





