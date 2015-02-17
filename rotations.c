#include "math.h"

const float FLT_EPSILON = 0.000001;

/* Type for rotation order info - see wiki for derivation details */
typedef struct RotOrderInfo {
    short axis[3];
    short parity; /* parity of axis permutation (even=0, odd=1) - 'n' in original code */
} RotOrderInfo;

/* Array of info for Rotation Order calculations
* WARNING: must be kept in same order as eEulerRotationOrders
*/
static const RotOrderInfo rotOrders[] = {
    /* i, j, k, n */
{{0, 1, 2}, 0}, /* XYZ */
{{0, 2, 1}, 1}, /* XZY */
{{1, 0, 2}, 1}, /* YXZ */
{{1, 2, 0}, 0}, /* YZX */
{{2, 0, 1}, 0}, /* ZXY */
{{2, 1, 0}, 1}  /* ZYX */
};

#define GET_ROTATIONORDER_INFO(order) (assert(order >= 0 && order <= 6), (order < 1) ? &rotOrders[0] : &rotOrders[(order) - 1])

/* returns two euler calculation methods, so we can pick the best */
static void mat3_to_eulo2(float M[3][3], float eul1[3], float eul2[3], const short order) {
    const RotOrderInfo *R = GET_ROTATIONORDER_INFO(order);
    short i = R->axis[0], j = R->axis[1], k = R->axis[2];
    float mat[3][3];
    float cy;

    /* process the matrix first */
    normalize_m3_m3(mat, M);

    cy = hypotf(mat[i][i], mat[i][j]);

    if (cy > 16.0f * FLT_EPSILON) {
        eul1[i] = atan2f(mat[j][k], mat[k][k]);
        eul1[j] = atan2f(-mat[i][k], cy);
        eul1[k] = atan2f(mat[i][j], mat[i][i]);

        eul2[i] = atan2f(-mat[j][k], -mat[k][k]);
        eul2[j] = atan2f(-mat[i][k], -cy);
        eul2[k] = atan2f(-mat[i][j], -mat[i][i]);
    }
    else {
        eul1[i] = atan2f(-mat[k][j], mat[j][j]);
        eul1[j] = atan2f(-mat[i][k], cy);
        eul1[k] = 0;

        copy_v3_v3(eul2, eul1);
    }

    if (R->parity) {
        negate_v3(eul1);
        negate_v3(eul2);
    }
}

/* Convert 3x3 matrix to Euler angles (in radians). */
void mat3_to_eulO(float eul[3], const short order, float M[3][3]) {
    float eul1[3], eul2[3];
    float d1, d2;

    mat3_to_eulo2(M, eul1, eul2, order);

    d1 = fabsf(eul1[0]) + fabsf(eul1[1]) + fabsf(eul1[2]);
    d2 = fabsf(eul2[0]) + fabsf(eul2[1]) + fabsf(eul2[2]);

    /* return best, which is just the one with lowest values it in */
    if (d1 > d2) {
        copy_v3_v3(eul, eul2);
    }
    else {
        copy_v3_v3(eul, eul1);
    }
}
/* skip error check, currently only needed by mat3_to_quat_is_ok */
void quat_to_mat3(float m[3][3], const float q[4]) {
    double q0, q1, q2, q3, qda, qdb, qdc, qaa, qab, qac, qbb, qbc, qcc;

    q0 = M_SQRT2 * (double)q[0];
    q1 = M_SQRT2 * (double)q[1];
    q2 = M_SQRT2 * (double)q[2];
    q3 = M_SQRT2 * (double)q[3];

    qda = q0 * q1;
    qdb = q0 * q2;
    qdc = q0 * q3;
    qaa = q1 * q1;
    qab = q1 * q2;
    qac = q1 * q3;
    qbb = q2 * q2;
    qbc = q2 * q3;
    qcc = q3 * q3;

    m[0][0] = (float)(1.0 - qbb - qcc);
    m[0][1] = (float)(qdc + qab);
    m[0][2] = (float)(-qdb + qac);

    m[1][0] = (float)(-qdc + qab);
    m[1][1] = (float)(1.0 - qaa - qcc);
    m[1][2] = (float)(qda + qbc);

    m[2][0] = (float)(qdb + qac);
    m[2][1] = (float)(-qda + qbc);
    m[2][2] = (float)(1.0 - qaa - qbb);
}

/* Convert quaternion to Euler angles (in radians). */
void quat_to_eulO(float e[3], short const order, const float q[4]){
    float M[3][3];

    quat_to_mat3(M, q);
    mat3_to_eulO(e, order, M);
}

/* Quaternions to Axis Angle */
void quat_to_axis_angle(float axis[3], float *angle, const float q[4]){
    float ha, si;

    /* calculate angle/2, and sin(angle/2) */
    ha = acosf(q[0]);
    si = sinf(ha);

    /* from half-angle to angle */
    *angle = ha * 2;

    /* prevent division by zero for axis conversion */
    if (fabsf(si) < 0.0005f)
        si = 1.0f;

    axis[0] = q[1] / si;
    axis[1] = q[2] / si;
    axis[2] = q[3] / si;
}

/******************************** Axis Angle *********************************/

void axis_angle_normalized_to_quat(float q[4], const float axis[3], const float angle)
{
    const float phi = 0.5f * angle;
    const float si = sinf(phi);
    const float co = cosf(phi);
    BLI_ASSERT_UNIT_V3(axis);
    q[0] = co;
    mul_v3_v3fl(q + 1, axis, si);
}

void axis_angle_to_quat(float q[4], const float axis[3], const float angle)
{
    float nor[3];

    if (LIKELY(normalize_v3_v3(nor, axis) != 0.0f)) {
        axis_angle_normalized_to_quat(q, nor, angle);
    }
    else {
        unit_qt(q);
    }
}

/* Axis Angle to Euler Rotation */
void axis_angle_to_eulO(float eul[3], const short order, const float axis[3], const float angle){
    float q[4];

    /* use quaternions as intermediate representation for now... */
    axis_angle_to_quat(q, axis, angle);
    quat_to_eulO(eul, order, q);
}

int main() {
    return 0;
}
