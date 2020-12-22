/**
 * Copyright (C) 2019-2020, Axis Communications AB, Lund, Sweden
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <bbox.h>

#include <unistd.h> // required for sleep

#include <iostream>
#include <stdexcept>

using namespace std;

/**
 * This example illustrates drawing on a single channel.
 *
 * 
 * The coordinate-space equals the visible area of the chosen channel.
 *
 *  ┏━━━━━━━━━━━━━━━━━━━━━━━━┓
 *  ┃                        ┃
 *  ┃ [0,0]                  ┃
 *  ┃   ┏━━━━━━━━━━┓         ┃
 *  ┃   ┃          ┃         ┃
 *  ┃   ┃ Channel1 ┃         ┃
 *  ┃   ┃          ┃         ┃
 *  ┃   ┗━━━━━━━━━━┛         ┃
 *  ┃            [1,1]       ┃
 *  ┃                        ┃
 *  ┃                        ┃
 *  ┗━━━━━━━━━━━━━━━━━━━━━━━━┛
 *
 * The intended usecase is performing video content analytics on one channel,
 * then draw bounding boxes with the same coordinate-space as was used for VCA.
 * 
 */

void
example_single_channel()
{
    // Draw on a single channel: 1
    bbox_t* bbox = bbox_new(1u, 1u);
    if (!bbox)
        throw runtime_error("Failed to create BBox");

    bbox_clear(bbox); // Remove all old bounding-boxes

    // Create all needed colors [These operations are slow!]
    bbox_color_t red  = bbox_color_from_rgb(0xff, 0x00, 0x00);
    bbox_color_t blue = bbox_color_from_rgb(0x00, 0x00, 0xff);

    bbox_style_outline(bbox);   // Switch to outline style
    bbox_thickness_thin(bbox);  // Switch to thin lines
    bbox_color(bbox, red);      // Switch to red [This operation is fast!]
    bbox_rectangle(bbox, 0.00, 0.00, 1.00, 1.00); // Draw a thin red outline rectangle

    bbox_style_corners(bbox);   // Switch to corners style
    bbox_thickness_thick(bbox); // Switch to thick lines
    bbox_color(bbox, blue);     // Switch to blue [This operation is fast!]
    bbox_rectangle(bbox, 0.40, 0.40, 0.60, 0.60); // Draw thick blue corners

    // Draw all queued geometry simultaneously
    if (!bbox_commit(bbox, 0u))
        throw runtime_error("Failed to commit single bbox");

    sleep(10); // To allow the user time to change view in order to observe example output.
    bbox_destroy(bbox);
}

/**
* This example illustrates drawing on multiple channels.
* The coordinate-space equals global/overview.
* 
* [0,0]
*   ┏━━━━━━━━━━━━━━━━━━━━━━━━┓
*   ┃  Channel1              ┃
*   ┃  ┏━━━━━━┓              ┃
*   ┃  ┃      ┃              ┃
*   ┃  ┃      ┃              ┃
*   ┃  ┗━━━━━━┛              ┃
*   ┃              Channel2  ┃
*   ┃              ┏━━━━━━┓  ┃
*   ┃              ┃      ┃  ┃
*   ┃              ┃      ┃  ┃
*   ┃              ┗━━━━━━┛  ┃
*   ┗━━━━━━━━━━━━━━━━━━━━━━━━┛
*                          [1,1]
* 
* The intended usecase is performing video content analytics on the entire image,
* then draw bounding boxes with the same coordinate-space as was used for VCA,
* and have it appear in all chosen channels simultaneously.
*
* Note that if you instead run VCA once per channel, i.e. multiple images,
* then you need to manually translate the coordinates to the global image space,
* before they can be drawn.
*/
void
example_multiple_channels()
{
    // Draw on channel 1 and 2
    bbox_t* bbox = bbox_new(2u, 1u, 2u);
    if (!bbox)
        throw runtime_error("Failed to create BBox");

    // This will not give errors on products which lack video-output!
    if (!bbox_video_output(bbox, true))
        throw runtime_error("Failed to enable video-output");

    bbox_clear(bbox);          // Remove all old bounding-boxes
    bbox_thickness_thin(bbox); // Use thin lines
    bbox_style_outline(bbox);  // Use outline style

    // Create all needed colors (These operations are slow!)
    bbox_color_t red  = bbox_color_from_rgb(0xff, 0x00, 0x00);
    bbox_color_t blue = bbox_color_from_rgb(0x00, 0x00, 0xff);

    bbox_color(bbox, red); // Switch to red (This operation is fast)
    bbox_rectangle(bbox, 0.00, 0.00, 1.00, 0.25); // Draw a red rectangle

    bbox_color(bbox, blue); // Switch to blue (This operation is fast)
    bbox_rectangle(bbox, 0.00, 0.75, 1.00, 1.00); // Draw a blue rectangle

    // Draw all queued geometry simultaneously
    if (!bbox_commit(bbox, 0u))
        throw runtime_error("Failed to commit multiple bbox");

    sleep(10);
    bbox_destroy(bbox);
}

int
test_known_pattern()
{
    int counter = 0;
    bbox_t* bbox = bbox_new(1u, 1u);
    if(!bbox)
        throw runtime_error("Failed to create BBox");

    // This is a NOP on products without video-output!
    if (!bbox_video_output(bbox, true))
        throw runtime_error("Failed to enable video-output");

    bbox_color_t col[3u] ={
        bbox_color_from_rgb(0xff, 0u, 0u),
        bbox_color_from_rgb(0u, 0xff, 0u),
        bbox_color_from_rgb(0u, 0u, 0xff),
    };

    bbox_clear(bbox);

    bbox_thickness_thin(bbox);
    bbox_thickness_thick(bbox);
    bbox_style_outline(bbox);
    bbox_style_corners(bbox);

    const float w = 1920.f;
    const float h = 1080.f;
    const float box_w = 100.f / w;
    const float box_h = 100.f / h;
    for (size_t loop = 0u; loop < 29u; loop += 1u) {
        for (size_t i = loop; i < loop + 32u; i += 1u) {
            float x = (200u) * (i%10u) / w;
            float y = (200u) * (i/10u) / h;
            bbox_color(bbox, col[(i + loop)%3u]);
            bbox_rectangle(bbox, x, y, x + box_w, y + box_h);
            counter++;
        }
        bbox_commit(bbox, 0u);
        sleep(1);
        bbox_clear(bbox);
    }
    bbox_destroy(bbox);
    return counter;
}

int main(void) try
{
    cout << "Bounding Box example" << endl;
    while (true) {
        cout << "Test Loop" << endl;
        example_single_channel();
        example_multiple_channels();
        int rectangles = test_known_pattern();
        cout << "Rectangles=" << rectangles << endl;
    }
    return EXIT_SUCCESS;
}
catch(const exception& e)
{
    cerr << e.what() << endl;
    return EXIT_FAILURE;
}