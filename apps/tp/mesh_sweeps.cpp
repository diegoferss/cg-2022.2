#include "mesh_sweeps.h"

#include <cassert>

namespace cg {

TriangleMesh* MakeSpiral(Polygon& generatrix, int num_subdiv,
                         float initial_radius, float num_revolutions,
                         float height_inc, float radius_inc,
                         bool draw_front_cap,
                         bool draw_back_cap, bool draw_generatrices) {
  assert(num_subdiv >= 3 && num_subdiv <= 40);
  // assert(initial_radius >= 2 * generatrix.Radius() && initial_radius <= 100);

  TriangleMesh::Data data;

  // subdiv+3 pq tem as tampas, subdiv+1 caso contr�rio
  // +2 no final por causa dos centros das tampas
  data.vertexCount = generatrix.Sides() * (num_subdiv + 3) + 2;
  // subdiv+1 pq tem as tampas, subdiv caso contr�rio
  data.triangleCount = 2 * generatrix.Sides() * (num_subdiv + 1);
  data.vertices = new vec3f[data.vertexCount];
  data.vertexNormals = new vec3f[data.vertexCount];
  data.triangles = new TriangleMesh::Triangle[data.triangleCount];

  int n_triangles{}, k{};

  const auto d_theta{360.0f * num_revolutions / float(num_subdiv)};

  // 0. Transladando geratriz p/ posi��o inicial da espiral
  generatrix.Translate({initial_radius, 0, 0});

  // 0.1. Criando tampa inicial
  auto cap_vertices = data.vertices + generatrix.Sides() * (num_subdiv + 1);
  auto cap_normals = data.vertexNormals + generatrix.Sides() * (num_subdiv + 1);
  auto cap_triangles = data.triangles + 2 * generatrix.Sides() * num_subdiv;

  // 0.1.1. Geratriz precisa ter no m�nimo 3 pts
  auto cap_normal = (generatrix.Center() - generatrix[0])
                        .cross(generatrix.Center() - generatrix[1])
                        .versor();

  int cap_center = cap_vertices - data.vertices;
  if (draw_front_cap) {
    *cap_vertices++ = generatrix.Center();
    *cap_normals++ = cap_normal;
    for (int i{}, tidx{cap_center + i}; i < generatrix.Sides(); i++, tidx++) {
      *cap_vertices++ = generatrix[i];
      *cap_normals++ = cap_normal;
      *cap_triangles++ = {
          cap_center, tidx + 1,
          (i == generatrix.Sides() - 1 ? cap_center + 1 : tidx + 2)};
    }
  }

  cg::Polygon tmp;

  for (int i{}; i <= num_subdiv; i++) {

    // 1. Adicionando v�rtices da i-�sima geratriz
    for (int j{}; j < generatrix.Sides(); j++) {
#pragma warning(disable : 6386)  // buffer overrun?????
      data.vertices[i * generatrix.Sides() + j] = generatrix[j];
      data.vertexNormals[i * generatrix.Sides() + j] =
          (generatrix[j] - generatrix.Center()).versor();
    }

    // 2. Guardando a i-�sima geratriz
    tmp = generatrix.Copy(generatrix);

    // 3. Transformando a i-�sima geratriz na (i + 1)-�sima geratriz:
    // 3.1. Rotacionando em torno de y
    generatrix.Rotate(d_theta, {0, 1, 0});

    // 3.2. Aplicando delta_e^h e delta_e^w
    vec3f xyz{generatrix.Center().versor() * radius_inc};
    xyz.y = height_inc * num_revolutions / num_subdiv;
    generatrix.Translate(xyz);

    // TODO: 4. Ligar i-�sima geratriz � (i + 1)-�sima
    for (int l{}; i < num_subdiv && l < generatrix.Sides(); l++, k++) {
      if (l == generatrix.Sides() - 1) {
        data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides(),
                                                  k + 1);
        data.triangles[n_triangles++].setVertices(k, k + 1,
                                                  k - generatrix.Sides() + 1);
      } else {
        data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides(),
                                                  k + generatrix.Sides() + 1);
#pragma warning(disable : 6385)  // buffer overrun?????
        data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides() + 1,
                                                  k + 1);
      }
    }
  }
  // 5. Gerando a segunda tampa
  // NUNCA
  // JAMAIS
  // TOCAR NESSE C�DIGO
  /// VAZA
  if (draw_back_cap) {
    cap_normal =
        (tmp.Center() - tmp[0]).cross(tmp.Center() - tmp[1]).versor().negate();
    cap_center = int(cap_vertices - data.vertices);
    *cap_vertices++ = tmp.Center();
    *cap_normals++ = cap_normal;
    for (int i{}, tidx{cap_center + i}; i < tmp.Sides(); i++, tidx++) {
      *cap_vertices++ = tmp[i];
      *cap_normals++ = cap_normal;
      *cap_triangles++ = {cap_center, tidx + 1,
                          (i == tmp.Sides() - 1 ? cap_center + 1 : tidx + 2)};
    }
  }

  return new TriangleMesh{std::move(data)};
}

TriangleMesh* MakeSpiral(Arc& generatrix, int num_subdiv,
    float initial_radius, float num_revolutions,
    float height_inc, float radius_inc,
    bool draw_front_cap,
    bool draw_back_cap, bool draw_generatrices) {
    assert(num_subdiv >= 3 && num_subdiv <= 40);
    // assert(initial_radius >= 2 * generatrix.Radius() && initial_radius <= 100);

    TriangleMesh::Data data;

    // subdiv+3 pq tem as tampas, subdiv+1 caso contr�rio
    // +2 no final por causa dos centros das tampas
    data.vertexCount = generatrix.Sides() * (num_subdiv + 3) + 2;
    // subdiv+1 pq tem as tampas, subdiv caso contr�rio
    data.triangleCount = 2 * generatrix.Sides() * (num_subdiv + 1);
    data.vertices = new vec3f[data.vertexCount];
    data.vertexNormals = new vec3f[data.vertexCount];
    data.triangles = new TriangleMesh::Triangle[data.triangleCount];

    int n_triangles{}, k{};

    const auto d_theta{ 360.0f * num_revolutions / float(num_subdiv) };

    // 0. Transladando geratriz p/ posi��o inicial da espiral
    generatrix.Translate({ initial_radius, 0, 0 });

    // 0.1. Criando tampa inicial
    auto cap_vertices = data.vertices + generatrix.Sides() * (num_subdiv + 1);
    auto cap_normals = data.vertexNormals + generatrix.Sides() * (num_subdiv + 1);
    auto cap_triangles = data.triangles + 2 * generatrix.Sides() * num_subdiv;

    // 0.1.1. Geratriz precisa ter no m�nimo 3 pts
    auto cap_normal = (generatrix.Center() - generatrix[0])
        .cross(generatrix.Center() - generatrix[1])
        .versor();

    int cap_center = cap_vertices - data.vertices;
    if (draw_front_cap) {
        vec3f direction = (generatrix[generatrix.Sides() - 1] - generatrix[0]) * 0.5;
        *cap_vertices++ = generatrix.angle() < 180 ? generatrix[0] + direction : generatrix.Center();
        *cap_normals++ = cap_normal;
        for (int i{}, tidx{ cap_center + i }; i < generatrix.Sides(); i++, tidx++) {
            *cap_vertices++ = generatrix[i];
            *cap_normals++ = cap_normal;
            *cap_triangles++ = {
                cap_center, tidx + 1,
                (i == generatrix.Sides() - 1 ? cap_center + 1 : tidx + 2) };
        }
    }

    cg::Arc tmp;

    for (int i{}; i <= num_subdiv; i++) {

        // 1. Adicionando v�rtices da i-�sima geratriz
        for (int j{}; j < generatrix.Sides(); j++) {
#pragma warning(disable : 6386)  // buffer overrun?????
            data.vertices[i * generatrix.Sides() + j] = generatrix[j];
            data.vertexNormals[i * generatrix.Sides() + j] =
                (generatrix[j] - generatrix.Center()).versor();
        }

        // 2. Guardando a i-�sima geratriz
        tmp = Arc::Copy(generatrix);

        // 3. Transformando a i-�sima geratriz na (i + 1)-�sima geratriz:
        // 3.1. Rotacionando em torno de y
        generatrix.Rotate(d_theta, { 0, 1, 0 });

        // 3.2. Aplicando delta_e^h e delta_e^w
        vec3f xyz{ generatrix.Center().versor() * radius_inc };
        xyz.y = height_inc * num_revolutions / num_subdiv;
        generatrix.Translate(xyz);

        // TODO: 4. Ligar i-�sima geratriz � (i + 1)-�sima
        for (int l{}; i < num_subdiv && l < generatrix.Sides(); l++, k++) {
            if (!generatrix.polylineClosed() && l == generatrix.Sides() - 1 && generatrix.angle() < 360) {
                continue;
            }
            if (l == generatrix.Sides() - 1) {
                data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides(),
                    k + 1);
                data.triangles[n_triangles++].setVertices(k, k + 1,
                    k - generatrix.Sides() + 1);
            }
            else {
                data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides(),
                    k + generatrix.Sides() + 1);
#pragma warning(disable : 6385)  // buffer overrun?????
                data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides() + 1,
                    k + 1);
            }
        }
    }
    // 5. Gerando a segunda tampa
    // NUNCA
    // JAMAIS
    // TOCAR NESSE C�DIGO
    /// VAZA
    cap_center = cap_vertices - data.vertices;
    if (draw_back_cap) {
        vec3f direction = (tmp[tmp.Sides() - 1] - tmp[0]) * 0.5;
        *cap_vertices++ = tmp.angle() < 180 ? tmp[0] + direction : tmp.Center();
        cap_normal =
            (tmp.Center() - tmp[0]).cross(tmp.Center() - tmp[1]).versor().negate();
        *cap_normals++ = cap_normal;
        for (int i{}, tidx{ cap_center + i }; i < tmp.Sides(); i++, tidx++) {
            *cap_vertices++ = tmp[i];
            *cap_normals++ = cap_normal;
            *cap_triangles++ = { cap_center, tidx + 1,
                                (i == tmp.Sides() - 1 ? cap_center + 1 : tidx + 2) };
        }
    }

    return new TriangleMesh{ std::move(data) };
}

TriangleMesh* MakeTwist(Polygon& generatrix, int num_subdiv,
                        float num_revolutions, float length_inc,
                        float vertical_translation,
                        float horizontal_translation, float initial_scale,
                        float final_scale,
                        bool draw_front_cap, bool draw_back_cap,
                        bool draw_generatrices) {
  assert(num_subdiv >= 1 && num_subdiv <= 50);
  // assert(vertical_translation >= -(initial_radius / 2) &&
  // vertical_translation <= (initial_radius / 2));
  // assert(horizontal_translation >= -(initial_radius / 2) &&
  // horizontal_translation <= (initial_radius / 2));

  TriangleMesh::Data data;

  data.vertexCount = generatrix.Sides() * (num_subdiv + 3) + 2;
  data.triangleCount = 2 * generatrix.Sides() * (num_subdiv + 1);
  data.vertices = new vec3f[data.vertexCount];
  data.vertexNormals = new vec3f[data.vertexCount];
  data.triangles = new TriangleMesh::Triangle[data.triangleCount];

  int n_triangles{};
  int k = 0;

  const auto d_theta{ 360.0f * num_revolutions / float(num_subdiv) };
  const auto d_length{ length_inc / float(num_subdiv) };
  const auto d_horizontal{ horizontal_translation / float(num_subdiv) };
  const auto d_vertical{ vertical_translation / float(num_subdiv) };

  // 0. Transladando geratriz p/ posi  o inicial do objeto
  generatrix.Translate({0, 0, 0});
  generatrix.AbsoluteScale(initial_scale);

  // 0.1. Criando tampa inicial
  auto cap_vertices = data.vertices + generatrix.Sides() * (num_subdiv + 1);
  auto cap_normals = data.vertexNormals + generatrix.Sides() * (num_subdiv + 1);
  auto cap_triangles = data.triangles + 2 * generatrix.Sides() * num_subdiv;

  // 0.1.1. Geratriz precisa ter no m�nimo 3 pts
  auto cap_normal = (generatrix.Center() - generatrix[0])
      .cross(generatrix.Center() - generatrix[1])
      .versor();

  int cap_center = cap_vertices - data.vertices;
  if (draw_front_cap) {
      *cap_vertices++ = generatrix.Center();
      *cap_normals++ = cap_normal;
      for (int i{}, tidx{ cap_center + i }; i < generatrix.Sides(); i++, tidx++) {
          *cap_vertices++ = generatrix[i];
          *cap_normals++ = cap_normal;
          *cap_triangles++ = {
              cap_center, tidx + 1,
              (i == generatrix.Sides() - 1 ? cap_center + 1 : tidx + 2) };
      }
  }

  // const auto initial_gen = Polygon::Copy(generatrix);

  cg::Polygon tmp;

  for (int i{}; i <= num_subdiv; i++) {

    // 1. Adicionando v�rtices da i-�sima geratriz
    for (int j{}; j < generatrix.Sides(); j++) {
#pragma warning(disable : 6386)  // buffer overrun?????
        data.vertices[i * generatrix.Sides() + j] = generatrix[j];
        data.vertexNormals[i * generatrix.Sides() + j] =
            (generatrix[j] - generatrix.Center()).versor();
    }

    // 1. Adicionando v rtices da i- sima geratriz
    for (int l{}; i < num_subdiv && l < generatrix.Sides(); l++, k++) {
        if (l == generatrix.Sides() - 1) {
            data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides(),
                k + 1);
            data.triangles[n_triangles++].setVertices(k, k + 1,
                k - generatrix.Sides() + 1);
        }
        else {
            data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides(),
                k + generatrix.Sides() + 1);
#pragma warning(disable : 6385)  // buffer overrun?????
            data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides() + 1,
                k + 1);
        }
    }

    // 2. Guardando a i- sima geratriz
    tmp = generatrix.Copy(generatrix);

    // 3. Transformando a i- sima geratriz na (i + 1)- sima geratriz:
    // 3.1. Rotacionando em torno de z
    generatrix.RotateInPlace(d_theta, {0, 0, 1});

    // 3.2. Aplicando l_v
    vec3f xyz{0, 0, d_length};
    generatrix.Translate(xyz);

    // 3.2.1 Aplicando o_w_v
    vec3f xyz_transl{ d_horizontal, -d_vertical, 0 };
    generatrix.Translate(xyz_transl);

    // 3.3 Aplicando s_b
    const auto d_scale{initial_scale +
                       ((final_scale - initial_scale) / float(num_subdiv)) *
                           (i + 1)};
    // printf("%f\n", d_scale);
    generatrix.AbsoluteScale(d_scale);
  }

  // 5. Gerando a segunda tampa
  // NUNCA
  // JAMAIS
  // TOCAR NESSE C�DIGO
  /// VAZA
  if (draw_back_cap) {
      cap_normal =
          -(tmp.Center() - tmp[0]).cross(tmp.Center() - tmp[1]).versor().negate();
      cap_center = int(cap_vertices - data.vertices);
      *cap_vertices++ = tmp.Center();
      *cap_normals++ = cap_normal;
      for (int i{}, tidx{ cap_center + i }; i < tmp.Sides(); i++, tidx++) {
          *cap_vertices++ = tmp[i];
          *cap_normals++ = cap_normal;
          *cap_triangles++ = { cap_center, tidx + 1,
                              (i == tmp.Sides() - 1 ? cap_center + 1 : tidx + 2) };
      }
  }

  return new TriangleMesh{std::move(data)};
}

TriangleMesh* MakeTwist(Arc& generatrix, int num_subdiv,
    float num_revolutions, float length_inc,
    float vertical_translation,
    float horizontal_translation, float initial_scale,
    float final_scale,
    bool draw_front_cap, bool draw_back_cap,
    bool draw_generatrices) {
    assert(num_subdiv >= 1 && num_subdiv <= 50);
    // assert(vertical_translation >= -(initial_radius / 2) &&
    // vertical_translation <= (initial_radius / 2));
    // assert(horizontal_translation >= -(initial_radius / 2) &&
    // horizontal_translation <= (initial_radius / 2));

    TriangleMesh::Data data;

    data.vertexCount = generatrix.Sides() * (num_subdiv + 3) + 2;
    data.triangleCount = 2 * generatrix.Sides() * (num_subdiv + 1);
    data.vertices = new vec3f[data.vertexCount];
    data.vertexNormals = new vec3f[data.vertexCount];
    data.triangles = new TriangleMesh::Triangle[data.triangleCount];

    int n_triangles{};
    int k = 0;

    const auto d_theta{ 360.0f * num_revolutions / float(num_subdiv) };
    const auto d_length{ length_inc / float(num_subdiv) };
    const auto d_horizontal{ horizontal_translation / float(num_subdiv) };
    const auto d_vertical{ vertical_translation / float(num_subdiv) };

    // 0. Transladando geratriz p/ posi  o inicial do objeto
    generatrix.Translate({ 0, 0, 0 });
    generatrix.AbsoluteScale(initial_scale);

    // 0.1. Criando tampa inicial
    auto cap_vertices = data.vertices + generatrix.Sides() * (num_subdiv + 1);
    auto cap_normals = data.vertexNormals + generatrix.Sides() * (num_subdiv + 1);
    auto cap_triangles = data.triangles + 2 * generatrix.Sides() * num_subdiv;

    // 0.1.1. Geratriz precisa ter no m�nimo 3 pts
    auto cap_normal = -(generatrix.Center() - generatrix[0])
        .cross(generatrix.Center() - generatrix[1])
        .versor();

    int cap_center = cap_vertices - data.vertices;
    if (draw_front_cap) {
        vec3f direction = (generatrix[generatrix.Sides() - 1] - generatrix[0]) * 0.5;
        *cap_vertices++ = generatrix.angle() < 180 ? generatrix[0] + direction : generatrix.Center();
        *cap_normals++ = cap_normal;
        for (int i{}, tidx{ cap_center + i }; i < generatrix.Sides(); i++, tidx++) {
            *cap_vertices++ = generatrix[i];
            *cap_normals++ = cap_normal;
            *cap_triangles++ = {
                cap_center, tidx + 1,
                (i == generatrix.Sides() - 1 ? cap_center + 1 : tidx + 2) };
        }
    }

    // const auto initial_gen = Polygon::Copy(generatrix);

    cg::Arc tmp;

    for (int i{}; i <= num_subdiv; i++) {

        // 1. Adicionando v�rtices da i-�sima geratriz
        for (int j{}; j < generatrix.Sides(); j++) {
#pragma warning(disable : 6386)  // buffer overrun?????
            data.vertices[i * generatrix.Sides() + j] = generatrix[j];
            data.vertexNormals[i * generatrix.Sides() + j] =
                (generatrix[j] - generatrix.Center()).versor();
        }

        // 1. Adicionando v rtices da i- sima geratriz
        for (int l{}; i < num_subdiv && l < generatrix.Sides(); l++, k++) {
            if (!generatrix.polylineClosed() && l == generatrix.Sides() - 1 && generatrix.angle() < 360) {
                continue;
            }
            if (l == generatrix.Sides() - 1) {
                data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides(),
                    k + 1);
                data.triangles[n_triangles++].setVertices(k, k + 1,
                    k - generatrix.Sides() + 1);
            }
            else {
                data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides(),
                    k + generatrix.Sides() + 1);
#pragma warning(disable : 6385)  // buffer overrun?????
                data.triangles[n_triangles++].setVertices(k, k + generatrix.Sides() + 1,
                    k + 1);
            }
        }

        // 2. Guardando a i- sima geratriz
        tmp = generatrix.Copy(generatrix);

        // 3. Transformando a i- sima geratriz na (i + 1)- sima geratriz:
        // 3.1. Rotacionando em torno de z
        generatrix.RotateInPlace(d_theta, { 0, 0, 1 });

        // 3.2. Aplicando l_v
        vec3f xyz{ 0, 0, d_length };
        generatrix.Translate(xyz);

        // 3.2.1 Aplicando o_w_v
        vec3f xyz_transl{ d_horizontal, -d_vertical, 0 };
        generatrix.Translate(xyz_transl);

        // 3.3 Aplicando s_b
        const auto d_scale{ initial_scale +
                           ((final_scale - initial_scale) / float(num_subdiv)) *
                               (i + 1) };
        // printf("%f\n", d_scale);
        generatrix.AbsoluteScale(d_scale);
    }

    // 5. Gerando a segunda tampa
    // NUNCA
    // JAMAIS
    // TOCAR NESSE C�DIGO
    /// VAZA
    cap_center = cap_vertices - data.vertices;
    if (draw_back_cap) {
        vec3f direction = (tmp[tmp.Sides() - 1] - tmp[0]) * 0.5;
        *cap_vertices++ = tmp.angle() < 180 ? tmp[0] + direction : tmp.Center();
        cap_normal =
            -(tmp.Center() - tmp[0]).cross(tmp.Center() - tmp[1]).versor().negate();
        *cap_normals++ = cap_normal;
        for (int i{}, tidx{ cap_center + i }; i < tmp.Sides(); i++, tidx++) {
            *cap_vertices++ = tmp[i];
            *cap_normals++ = cap_normal;
            *cap_triangles++ = { cap_center, tidx + 1,
                                (i == tmp.Sides() - 1 ? cap_center + 1 : tidx + 2) };
        }
    }

    return new TriangleMesh{ std::move(data) };
}

}  // namespace cg