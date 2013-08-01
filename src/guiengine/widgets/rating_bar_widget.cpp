//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2009 Marianne Gagnon
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "guiengine/engine.hpp"
#include "guiengine/modaldialog.hpp"
#include "guiengine/widgets/rating_bar_widget.hpp"
#include "utils/string_utils.hpp"
#include <string.h>

#include <IGUIEnvironment.h>
#include <IGUIElement.h>
#include <IGUIButton.h>
#include <cmath>

using namespace GUIEngine;
using namespace irr::core;
using namespace irr;

// -----------------------------------------------------------------------------
RatingBarWidget::RatingBarWidget() : Widget(WTYPE_RATINGBAR)
{
    m_rating = 0;
    m_stars = 3;
    m_steps = 3;
    for(int i = 0; i < m_stars; i++)
        m_star_values.push_back(0);
}

// -----------------------------------------------------------------------------
void RatingBarWidget::add()
{
    irr::core::rect<s32> widget_size = rect<s32>(m_x, m_y, m_x + m_w, m_y + m_h);
    m_element = GUIEngine::getGUIEnv()->addButton(widget_size, m_parent, getNewNoFocusID(), NULL, L"");
    m_id = m_element->getID();
    m_element->setTabStop(false);
    m_element->setTabGroup(false);
}

// -----------------------------------------------------------------------------
/** Get the current step of the star
 * 
 * \param index     The index of the star.
 * \return The current step of the star.
 */
int RatingBarWidget::getStepsOfStar(int index)
{
    assert(index >= 0 && index < m_stars); // Index must be between 0 and m_star_number - 1.

    return m_star_values[index];
} // getStepOfStar



void RatingBarWidget::setStepValues(float float_rating)
{
    int rating = round(float_rating);
    float step_size = 1 / (float)(m_steps - 1);

    for (int star = 0; star < m_stars; star++)
    {
        if (rating < star)
            m_star_values[star] = 0;
        else if (rating > star + 1)
            m_star_values[star] = m_steps-1;
        else
        {
            for (int step = 0; step < m_steps; step++)
            {
                if (rating > star + step_size * (step - 0.5) && rating < star + step_size * (step + 0.5))
                {
                    m_star_values[star] = step;
                    break;
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------

void RatingBarWidget::setRating(float rating)
{
    m_rating = rating;
    setStepValues(m_rating);
}

// -----------------------------------------------------------------------------

void RatingBarWidget::setStepValuesByMouse(const core::position2d<s32> & mouse_position)
{
    if(m_element->getAbsolutePosition().isPointInside(mouse_position))
    {

        float value = (float)(mouse_position.X - m_element->getAbsolutePosition().UpperLeftCorner.X);
        setStepValues(  (float)( value / (float)m_w * (float)m_stars)  );

    }
}



