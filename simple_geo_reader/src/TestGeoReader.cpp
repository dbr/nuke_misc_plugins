/*
 Example geo reader
 */

// The class must match the compiled filename (e.g TestGeoReader.dylib)
static const char* const CLASS = "TestGeoReader";
static const char* const HELP = "Simple example geo reader";

#include "DDImage/SourceGeo.h"
#include "DDImage/Triangle.h"
#include "DDImage/Knobs.h"

using namespace DD::Image;

class TestGeoReader : public SourceGeo
{
private:
    // local matrix that Axis_Knob fills in
    Matrix4 _local;
    Knob* _pAxisKnob;

protected:
    void _validate(bool for_real)
    {
        SourceGeo::_validate(for_real);
    }

public:
    static const Description description;
    const char* Class() const { return CLASS; }
    const char* node_help() const { return HELP; }

    TestGeoReader(Node* node) : SourceGeo(node)
    {
        _local.makeIdentity();
        _pAxisKnob = NULL;
    }

    void knobs(Knob_Callback f)
    {
        // Default geo knobs
        SourceGeo::knobs(f);
        Newline(f);

        // TODO: File knob

        Divider(f);
        // transform knobs
        _pAxisKnob = Axis_knob(f, &_local, "transform");

        if (_pAxisKnob != NULL) {
            if (GeoOp::selectable() == true)
                _pAxisKnob->enable();
            else
                _pAxisKnob->disable();
        }
    }

    // The will handle the knob changes.
    int knob_changed(Knob* k)
    {
        if (k != NULL) {
            if (strcmp(k->name(), "selectable") == 0) {
            if (GeoOp::selectable() == true)
                _pAxisKnob->enable();
            else
                _pAxisKnob->disable();
                return 1;
            }
        }

        return SourceGeo::knob_changed(k);
    }

    // Hash up knobs that affect the Sphere:
    void get_geometry_hash()
    {
        SourceGeo::get_geometry_hash();   // Get all hashes up-to-date

        // Knobs that change the geometry structure:
        //geo_hash[Group_Primitives].append(radius);

        // Knobs that change the point locations:
        //geo_hash[Group_Points].append(radius);

        // Knobs that change the vertex attributes:
        //geo_hash[Group_Attributes].append(my_u_extent);

        geo_hash[Group_Matrix].append(_local.a00);
        geo_hash[Group_Matrix].append(_local.a01);
        geo_hash[Group_Matrix].append(_local.a02);
        geo_hash[Group_Matrix].append(_local.a03);

        geo_hash[Group_Matrix].append(_local.a10);
        geo_hash[Group_Matrix].append(_local.a11);
        geo_hash[Group_Matrix].append(_local.a12);
        geo_hash[Group_Matrix].append(_local.a13);

        geo_hash[Group_Matrix].append(_local.a20);
        geo_hash[Group_Matrix].append(_local.a21);
        geo_hash[Group_Matrix].append(_local.a22);
        geo_hash[Group_Matrix].append(_local.a23);

        geo_hash[Group_Matrix].append(_local.a30);
        geo_hash[Group_Matrix].append(_local.a31);
        geo_hash[Group_Matrix].append(_local.a32);
        geo_hash[Group_Matrix].append(_local.a33);
    }

    void geometry_engine(Scene& scene, GeometryList& out)
    {
        // Apply the concat matrix to all the GeoInfos.
      
        SourceGeo::geometry_engine(scene, out);

        // multiply the node matrix
        for (unsigned i = 0; i < out.size(); i++)
            out[i].matrix = _local * out[i].matrix;
  }

    void create_geometry(Scene& scene, GeometryList& out)
    {
        // Geo is defined as a face-vetrex mesh:
        // http://en.wikipedia.org/wiki/Polygon_mesh#Face-vertex_meshes
        int obj = 0;

        // Create polygons
        if(rebuild(Mask_Primitives)){
            out.delete_objects();
            out.add_object(obj);
            
            // Connect vertex 0, 1 and 2 together with a triangle
            out.add_primitive(obj, new Triangle(0, 1, 2));

            // Going to specify points (positions) and attributes (UV's etc)
            set_rebuild(Mask_Points | Mask_Attributes);
        }

        unsigned int num_points = 3;

        // Set vertex positions
        if (rebuild(Mask_Points)) {
            PointList* points = out.writable_points(obj);
            points->resize(num_points);

            int p = 0;
            (*points)[p].set(0, 0, 0);
            ++p;
            (*points)[p].set(1, 0, 0);
            ++p;
            (*points)[p].set(1, 1, 1);
            ++p;
        }

        // Set vertex normals and UV
        if (rebuild(Mask_Attributes)) {
            // Normals
            GeoInfo& info = out[obj];

            Attribute* N = out.writable_attribute(obj, Group_Points, "N", NORMAL_ATTRIB);

            for (unsigned p = 0; p < num_points; p++) {
                N->normal(p) = Vector3(0.0, 1.0, 0.5);
            }

            // UV
            const Primitive** PRIMS = info.primitive_array();
            Attribute* uv = out.writable_attribute(obj, Group_Vertices, "uv", VECTOR4_ATTRIB);
            unsigned v = (*PRIMS++)->vertex_offset();
            // First two x,y arguments are the UV coordinates, not sure if the
            // second two arguments are relevant
            uv->vector4(v++).set(0.0f, 0.0f, 0.0f, 1.0f);
            uv->vector4(v++).set(1.0f, 0.0f, 0.0f, 1.0f);
            uv->vector4(v++).set(1.0f, 1.0f, 0.0f, 1.0f);
        }
  }

  void build_handles(ViewerContext* ctx)
  {
    // call build_matrix_handle to multiply the context model matrix with the local matrix so the
    // nodes above it will display correctly
    build_matrix_handles(ctx, _local);
  }
};

static Op* build(Node* node) { return new TestGeoReader(node); }
const Op::Description TestGeoReader::description(CLASS, build);
