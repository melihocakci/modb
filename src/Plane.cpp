#include <modb/Plane.h>
#include <modb/CopyFrom.h>
#include <cstring>

modb::Plane::Plane(std::string oid, modb::Point baseLocation, modb::Point mbrLocation, double mbrWidth):
    m_oid{ oid }, m_mbrWidth{ mbrWidth } {
    m_baseLocation = baseLocation;
    m_mbrLocation = mbrLocation;
}

modb::Plane::Plane(uint8_t* buffer): m_baseLocation{}, m_mbrLocation{} {
    modb::CopyFrom copier{};

    copier(buffer, &m_oid, sizeof(m_oid));

    copier(buffer, &m_baseLocation.longitude, sizeof(m_baseLocation.longitude));

    copier(buffer, &m_baseLocation.latitude, sizeof(m_baseLocation.latitude));

    copier(buffer, &m_mbrLocation.longitude, sizeof(m_mbrLocation.longitude));

    copier(buffer, &m_mbrLocation.latitude, sizeof(m_mbrLocation.latitude));

    copier(buffer, &m_mbrWidth, sizeof(m_mbrWidth));
}

size_t modb::Plane::fillBuffer(uint8_t* buffer) {
    modb::CopyFrom copier{};

    copier(&m_oid, buffer, sizeof(m_oid));

    copier(&m_baseLocation.longitude, buffer, sizeof(m_baseLocation.longitude));

    copier(&m_baseLocation.latitude, buffer, sizeof(m_baseLocation.latitude));

    copier(&m_mbrLocation.longitude, buffer, sizeof(m_mbrLocation.longitude));

    copier(&m_mbrLocation.latitude, buffer, sizeof(m_mbrLocation.latitude));

    copier(&m_mbrWidth, buffer, sizeof(m_mbrWidth));

    return copier.getOffset();
}

std::string modb::Plane::getOid() { return m_oid; }