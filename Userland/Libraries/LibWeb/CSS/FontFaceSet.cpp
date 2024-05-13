/*
 * Copyright (c) 2024, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibJS/Heap/Heap.h>
#include <LibJS/Runtime/Realm.h>
#include <LibWeb/Bindings/FontFaceSetPrototype.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/CSS/FontFaceSet.h>
#include <LibWeb/WebIDL/Promise.h>

namespace Web::CSS {

JS_DEFINE_ALLOCATOR(FontFaceSet);

JS::NonnullGCPtr<FontFaceSet> FontFaceSet::construct_impl(JS::Realm& realm, Vector<JS::Handle<FontFace>> initial_faces)
{
    auto promise = WebIDL::create_promise(realm);
    return realm.heap().allocate<FontFaceSet>(realm, realm, promise, move(initial_faces));
}

JS::NonnullGCPtr<FontFaceSet> FontFaceSet::create(JS::Realm& realm)
{
    return construct_impl(realm, {});
}

FontFaceSet::FontFaceSet(JS::Realm& realm, JS::NonnullGCPtr<WebIDL::Promise> ready_promise, Vector<JS::Handle<FontFace>>)
    : Bindings::PlatformObject(realm)
    , m_ready_promise(ready_promise)
{
    // FIXME: Only set this after all the initial faces have been loaded
    m_status = Bindings::FontFaceSetLoadStatus::Loaded;
    // FIXME: Only resolve the promise after all the initial faces have been loaded
    WebIDL::resolve_promise(realm, *m_ready_promise);
}

void FontFaceSet::initialize(JS::Realm& realm)
{
    Base::initialize(realm);

    WEB_SET_PROTOTYPE_FOR_INTERFACE(FontFaceSet);
}

void FontFaceSet::visit_edges(Cell::Visitor& visitor)
{
    Base::visit_edges(visitor);
    visitor.visit(m_ready_promise);
}

// https://drafts.csswg.org/css-font-loading/#dom-fontfaceset-add
JS::NonnullGCPtr<FontFaceSet> FontFaceSet::add(JS::Handle<FontFace>)
{
    // FIXME: Do the steps
    return *this;
}

// https://drafts.csswg.org/css-font-loading/#dom-fontfaceset-load
JS::ThrowCompletionOr<JS::NonnullGCPtr<JS::Promise>> FontFaceSet::load(String const&, String const&)
{
    // FIXME: Do the steps
    auto promise = WebIDL::create_rejected_promise(realm(), WebIDL::NotSupportedError::create(realm(), "FontFaceSet::load is not yet implemented"_fly_string));
    return verify_cast<JS::Promise>(*promise->promise());
}

// https://drafts.csswg.org/css-font-loading/#font-face-set-ready
JS::NonnullGCPtr<JS::Promise> FontFaceSet::ready() const
{
    return verify_cast<JS::Promise>(*m_ready_promise->promise());
}

}
